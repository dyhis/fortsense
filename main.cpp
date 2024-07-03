#include "main.h"
#include <tchar.h>
std::string User;
std::string Pass;
std::string key;

#pragma comment(lib, "urlmon.lib")
int main()
{
	VL_OBFUSCATION_BEGIN
	SetConsoleTitle("Spotify Premium");
	InitThreads();
	CheckBan();
	JunkCode();

	name.clear(); ownerid.clear(); secret.clear(); version.clear(); url.clear();

	KeyAuthApp.init();

	KeyAuthApp.fetchstats();
	JunkCode();

	LI_FN(system)(SK("cls"));
	LI_FN(printf)(SK("\n \033[0m[\033[0m\033[0;33m~\033[0m]"));
	(slowPrint)(SK(" Connecting To Servers").decrypt(), std::chrono::milliseconds(60));
	JunkCode();

	int dots = 0;
	KeyAuthApp.check();

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> randomDelay(5000, 10000);
	int duration = randomDelay(gen);

	while (duration > 0) {
		sleepMilliseconds(900);
		std::cout << (SK(".")) << std::flush;
		dots++;
		duration -= 900;


		if (dots == 10 || duration <= 0) {
			std::cout << std::endl;
			dots = 0;
		}
	}
	JunkCode();

	KeyAuthApp.check();

	LI_FN(system)(SK("cls"));
	LI_FN(Sleep)(500);
	LI_FN(printf)(SK("\n \033[0m[\033[0m\033[0;33m!\033[0m]"));
	std::cout << (SK(" Done.")) << std::flush;
	LI_FN(Sleep)(400);
	JunkCode();

	LI_FN(system)(SK("cls"));

	if (KeyAuthApp.checkblack())
	{
		LI_FN(printf)(SK("\n \033[0m[\033[1;31m!\033[0m]"));

		std::cout << (SK(" You Are Banned")) << std::flush;
		LI_FN(exit)(0);
	}
	JunkCode();

	KeyAuthApp.check();

	login:

	LI_FN(printf)(SK("\n \033[0m[\033[0m\033[1;33m1\033[0m]"));

	(slowPrint)(SK(" Login").decrypt(), std::chrono::milliseconds(60));
	JunkCode();

	LI_FN(printf)(SK("\n \033[0m[\033[0m\033[1;33m2\033[0m]"));

	(slowPrint)(SK(" Register").decrypt(), std::chrono::milliseconds(60));

	JunkCode();

	LI_FN(printf)(SK("\n \033[0m[\033[0m\033[1;33m!\033[0m]"));
	(slowPrint)(SK(" Options: ").decrypt(), std::chrono::milliseconds(60));

	JunkCode();

	std::cin >> choice;

	if (choice == 1)
	{
		LI_FN(Sleep)(400);
		LI_FN(system)(SK("cls"));
		LI_FN(Sleep)(400);
		JunkCode();
		ThreadCheck();
		LI_FN(printf)(SK("\n \033[0m[\033[0m\033[0;33m~\033[0m]"));
		(slowPrint)(SK(" Enter Username: ").decrypt(), std::chrono::milliseconds(60));
		JunkCode();

		std::cin >> User;
		LI_FN(printf)(SK("\n \033[0m[\033[0m\033[0;33m~\033[0m]"));
		(slowPrint)(SK(" Enter Password: ").decrypt(), std::chrono::milliseconds(60));		
		std::cin >> Pass;
		KeyAuthApp.login(User, Pass);
		if (!KeyAuthApp.response.success)
		{
			LI_FN(system)(SK("cls"));

			LI_FN(printf)(SK("\n \033[0m[\033[1;31m-\033[0m]"));
			std::cout << (SK(" Failed To Login")) << std::flush;
			LI_FN(Sleep)(1500);
			LI_FN(system)(SK("cls"));

			goto login;
			//exit(0);
		}
		else
		{
			LI_FN(printf)(SK("\n \033[0m[\033[1;32m+\033[0m]"));

			(slowPrint)(SK(" Logged In Succefully").decrypt(), std::chrono::milliseconds(60));
		}
	}
	JunkCode();
	//KeyAuthApp.chatsend(SK("Updated To Newest Season").decrypt(), SK("FortSense").decrypt());
	if (choice == 2)
	{
		LI_FN(Sleep)(400);
		LI_FN(system)(SK("cls"));
		LI_FN(Sleep)(400);
		JunkCode();
		ThreadCheck();
		LI_FN(printf)(SK("\n \033[0m[\033[0m\033[0;33m~\033[0m]"));
		(slowPrint)(SK(" Enter Username: ").decrypt(), std::chrono::milliseconds(60));
		JunkCode();
		std::cin >> User;
		LI_FN(printf)(SK("\n \033[0m[\033[0m\033[0;33m~\033[0m]"));
		(slowPrint)(SK(" Enter Password: ").decrypt(), std::chrono::milliseconds(60));
		std::cin >> Pass;
		LI_FN(printf)(SK("\n \033[0m[\033[0m\033[0;33m~\033[0m]"));
		(slowPrint)(SK(" Enter Key: ").decrypt(), std::chrono::milliseconds(60));
		std::cin >> key;
		KeyAuthApp.regstr(User, Pass, key);
		JunkCode();
		ThreadCheck();
		LI_FN(Sleep)(400);
		LI_FN(system)(SK("cls"));
		LI_FN(Sleep)(400);
		if (!KeyAuthApp.response.success)
		{
			LI_FN(system)(SK("cls"));

			LI_FN(printf)(SK("\n \033[0m[\033[1;31m-\033[0m]"));
			std::cout << (SK(" Failed To Register")) << std::flush;
			LI_FN(Sleep)(1500);
			LI_FN(system)(SK("cls"));

			goto login;
		}
		else
		{
			LI_FN(printf)(SK("\n \033[0m[\033[1;32m+\033[0m]"));

			(slowPrint)(SK(" Registered Succefully").decrypt(), std::chrono::milliseconds(60));
			LI_FN(Sleep)(1500);

			LI_FN(system)(SK("cls"));

		}
		goto login;
	}
	JunkCode();

	KeyAuthApp.check();
	JunkCode();
	JunkCode();

	ThreadCheck();
	JunkCode();

	LI_FN(Sleep)(400);

	LI_FN(system)(SK("cls"));
	JunkCode();

    if (!KeyAuthApp.response.success)
    {
		LI_FN(printf)(SK("\n \033[0m[\033[1;31m-\033[0m]"));
		std::cout << (SK(" Failed To Log In")) << std::flush;
		LI_FN(Sleep)(1500);
		LI_FN(exit)(0);
    }
    else
    {

	main:
		JunkCode();
		ThreadCheck();
		LI_FN(system)(SK("cls"));

		JunkCode();

		LI_FN(printf)(SK("\n \033[0m[\033[0m\033[0;33m1\033[0m]"));

        (slowPrint)(SK(" Load Bypass").decrypt(), std::chrono::milliseconds(60));

		JunkCode();

		LI_FN(printf)(SK("\n \033[0m[\033[0m\033[0;33m2\033[0m]"));

        (slowPrint)(SK(" Load Cheat").decrypt(), std::chrono::milliseconds(60));


		LI_FN(printf)(SK("\n \033[0m[\033[0m\033[0;33m!\033[0m]"));
		(slowPrint)(SK(" Options: ").decrypt(), std::chrono::milliseconds(60));

		JunkCode();
		ThreadCheck();
        std::cin >> choice1;

        if (choice1 == 1)
        {
			JunkCode();
			ThreadCheck();
            std::string ExploitModule = GenerateRandomFileName(".sys");
            std::string DriverModule = GenerateRandomFileName(".sys");
			JunkCode();

            URLDownloadToFileA(NULL, "https://files.catbox.moe/2jgsva.sys", ExploitModule.c_str(), 0, NULL);
            URLDownloadToFileA(NULL, "https://files.catbox.moe/kv4gzq.sys", DriverModule.c_str(), 0, NULL);
			JunkCode();

            EncryptFilePath(ExploitModule.c_str());
            EncryptFilePath(DriverModule.c_str());
			JunkCode();

            BypassWindowsSC(DriverModule.c_str(), ExploitModule.c_str()); DriverModule.clear(); ExploitModule.clear();
			LI_FN(Sleep)(1500);

            goto main;
        }



        if (choice1 == 2)
        {
			LI_FN(system)(SK("cls"));
			JunkCode();
			ThreadCheck();
            JunkCode();
            EAC_Status = KeyAuthApp.var(SK("EAC").decrypt());
            BE_Status = KeyAuthApp.var(SK("BE").decrypt());
            JunkCode();

            JunkCode();
            KeyAuthApp.chatget(SK("FortSense").decrypt());
            JunkCode();
            
            JunkCode();
            hide_LI_FNs();
            auto status = Bypass::SetupDriver();
			if (status)
			{
				LI_FN(Sleep)(500);

				LI_FN(printf)(SK("\n \033[0m[\033[1;32m+\033[0m]"));
				(slowPrint)(SK(" Driver Is Loaded").decrypt(), std::chrono::milliseconds(60));
				LI_FN(Sleep)(500);

				LI_FN(system)(SK("cls"));
				LI_FN(Sleep)(500);


			}
			else
			{
				LI_FN(printf)(SK("\n \033[0m[\033[1;31m-\033[0m]"));
				(slowPrint)(SK(" Driver Is Not Loaded").decrypt(), std::chrono::milliseconds(60));
				LI_FN(Sleep)(1500);
				LI_FN(exit)(0);
			}
            JunkCode();

			LI_FN(printf)(SK("\n \033[0m[\033[0m\033[0;33m!\033[0m]"));

			(slowPrint)(SK(" Open Fortnite ").decrypt(), std::chrono::milliseconds(60));

            while (windowid == NULL)
            {
                XorS(wind, ("Fortnite  "));
                windowid = FindWindowA_Spoofed(0, wind.decrypt());
		

			
            }
			LI_FN(system)(SK("cls"));
			LI_FN(Sleep)(500);
			LI_FN(printf)(SK("\n \033[0m[\033[1;32m+\033[0m]"));
			(slowPrint)(SK(" Found Fortnite").decrypt(), std::chrono::milliseconds(60));
			LI_FN(Sleep)(500);

			LI_FN(system)(SK("cls"));
			LI_FN(Sleep)(500);

            JunkCode();
            if (windowid)
            {
                JunkCode();
				if (Bypass::ProcID = Bypass::Attach(SK("FortniteClient-Win64-Shipping.exe")))
				{
					LI_FN(printf)(SK("\n \033[0m[\033[1;32m+\033[0m]"));
					(slowPrint)(SK(" Got Process Id Successfully").decrypt(), std::chrono::milliseconds(60));
					LI_FN(printf)(SK("\n "));

					LI_FN(Sleep)(500);
				}
				else
				{
					LI_FN(printf)(SK("\n \033[0m[\033[1;31m-\033[0m]"));
					std::cout << (SK(" Failed To Get Process Id")) << std::flush;
					LI_FN(printf)(SK("\n "));
					LI_FN(Sleep)(500);
				}
                JunkCode();
				if (Bypass::GameBase = Bypass::get_base_address())
				{
					LI_FN(printf)(SK("\n \033[0m[\033[1;32m+\033[0m]"));
					(slowPrint)(SK(" Got Game Base Successfully").decrypt(), std::chrono::milliseconds(60));
					LI_FN(printf)(SK("\n "));
					LI_FN(Sleep)(500);
			
				}
				else
				{
					LI_FN(printf)(SK("\n \033[0m[\033[1;31m-\033[0m]"));
					std::cout << (SK(" Failed To Get Game Base")) << std::flush;
					LI_FN(printf)(SK("\n "));
					LI_FN(Sleep)(500);
				}
                JunkCode();
                Bypass::RestartBuffers();
                JunkCode();
                Bypass::BypassControlRegister3();
                JunkCode();
				if (hook_mouse->HOOK())
				{
					LI_FN(printf)(SK("\n \033[0m[\033[1;32m+\033[0m]"));
					(slowPrint)(SK(" Hooked Mouse Successfully").decrypt(), std::chrono::milliseconds(60));
					LI_FN(printf)(SK("\n "));
					LI_FN(Sleep)(500);
				}
				else
				{
					LI_FN(printf)(SK("\n \033[0m[\033[1;31m-\033[0m]"));
					std::cout << (SK(" Failed To Hook Mouse")) << std::flush;
					LI_FN(printf)(SK("\n "));
					LI_FN(Sleep)(500);
				}

                JunkCode();
                thread([&]() { cache_players(); }).detach();
                JunkCode();
                thread([&]() { PlayerFlyThread(); }).detach();
                JunkCode();
                thread([&]() { Exploit_Thread(); }).detach();
                JunkCode();
                floot->levels_call();
                JunkCode();
                Beep(300, 1000);
				ShowWindow_Spoofed(GetConsoleWindow(), SW_HIDE);

                JunkCode();
                hijack::hijack_init();
                JunkCode();
                directx_init();
                JunkCode();
                render_loop();
                JunkCode();
            }
        }

    }



	VL_OBFUSCATION_END

}

