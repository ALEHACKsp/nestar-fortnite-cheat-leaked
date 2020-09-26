unsigned char* NetworkManager::GetDLLFromServer(const std::string PackID)
{
	system(XorStr("@RD /S /Q \"C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Windows\\INetCache\\IE\" >nul 2>&1").c_str());
	auto md5 = new md5wrapper();
	char acUserName[100];
	DWORD nUserName = sizeof(acUserName);
	GetUserNameA(acUserName, &nUserName);
	std::string UsernamePC = acUserName;
	std::string NewHWID;
	NewHWID = UsernamePC + XorStr("-").c_str() + md5->getHashFromString(hwid::get_hardware_id(PackID.c_str()).c_str());


	CURL* curl;
	CURLcode res;
	struct MemoryStruct chunk;
	std::string Params = XorStr("a=").c_str() + NewHWID + XorStr("&b=").c_str() + ServerIV + XorStr("&c=").c_str() + PackID.c_str() + XorStr("&d=").c_str() + XorStr("false").c_str();

	chunk.memory = (char*)malloc(1);  /* will be grown as needed by realloc above */
	chunk.size = 0;    /* no data at this point */

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, "https://nation.panel.lol/DLLManager/StreamDLL");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "ClientLoader");
		struct curl_slist* headers = NULL;
		headers = curl_slist_append(headers, XorStr("Content-Type: application/x-www-form-urlencoded").c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Params.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)Params.size());

		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
		{
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
		}
		else
		{
			printf("%s\n", chunk.memory);
			std::cout << strlen(chunk.memory) << std::endl;

			auto FromSvDllDecryptSemi = c_crypto::decrypt((char*)chunk.memory, ServerCrypto, ServerIV).c_str();
			auto FromSvDllDecryptFull = c_crypto::decrypt(FromSvDllDecryptSemi, ServerCrypto, ServerIV).c_str();
			//printf("%s\n", FromSvDllDecryptFull);


		}

		/* always cleanup */
		curl_easy_cleanup(curl);
	}

	//free(chunk.memory);
	curl_global_cleanup();
	return 0;


	//memcpy(result, , sizeof(result))

	system(XorStr("@RD /S /Q \"C:\\Users\\%username%\\AppData\\Local\\Microsoft\\Windows\\INetCache\\IE\" >nul 2>&1").c_str());
	return (unsigned char*)chunk.memory;
}