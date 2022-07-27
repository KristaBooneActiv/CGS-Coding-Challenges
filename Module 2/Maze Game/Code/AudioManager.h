#pragma once
#include <windows.h>
#include <Mmsystem.h>
#include <mciapi.h>

class AudioManager
{
	static AudioManager* s_pInstance;

	bool m_SoundOn;

	AudioManager()
		: m_SoundOn(true)
	{

	}

public:
	static AudioManager* GetInstance()
	{
		if (s_pInstance == nullptr)
		{
			s_pInstance = new AudioManager();
		}
		return s_pInstance;
	}

	static void DestroyInstance()
	{
		delete s_pInstance;
		s_pInstance = nullptr;
	}

	void ToggleSound()
	{
		m_SoundOn = !m_SoundOn;
	}

	bool IsSoundOn()
	{
		return m_SoundOn;
	}

	void PlayMainTheme()
	{
		if (!m_SoundOn)
			return;

		StopCurrentTheme();

		// Source: Steven O�Brien - licensed under a Creative Commons License
		mciSendString(L"open \"Sound\\MainTheme.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
		mciSendString(L"play mp3 repeat", NULL, 0, NULL);
	}

	void StopCurrentTheme()
	{
		if (!m_SoundOn)
			return;
		mciSendString(L"stop mp3", NULL, 0, NULL);
	}

	void PlayDoorClosedSound()
	{
		if (!m_SoundOn)
			return;
		// Source: Royalty-free Soniss GameAudioGDC packages
		PlaySound(L"Sound\\DoorLocked.wav", GetModuleHandle(NULL), SND_ASYNC);
	}

	void PlayDoorOpenSound()
	{
		if (!m_SoundOn)
			return;
		// Source: Royalty-free Soniss GameAudioGDC packages
		PlaySound(L"Sound\\DoorUnlock.wav", GetModuleHandle(NULL), SND_ASYNC);
	}

	void PlayKeyPickupSound()
	{
		if (!m_SoundOn)
			return;
		// Source: Royalty-free Soniss GameAudioGDC packages
		PlaySound(L"Sound\\KeyPickup.wav", GetModuleHandle(NULL), SND_ASYNC);
	}

	void PlayKeyDropSound()
	{
		if (!m_SoundOn)
			return;
		// Source: Royalty-free Soniss GameAudioGDC packages
		PlaySound(L"Sound\\KeyDrop.wav", GetModuleHandle(NULL), SND_ASYNC);
	}

	void PlayMoneySound()
	{
		if (!m_SoundOn)
			return;
		// Source: Royalty-free Soniss GameAudioGDC packages
		PlaySound(L"Sound\\Money.wav", GetModuleHandle(NULL), SND_ASYNC);
	}

	void PlayLoseLivesSound()
	{
		if (!m_SoundOn)
			return;
		// Source: Royalty-free Soniss GameAudioGDC packages
		PlaySound(L"Sound\\Damage.wav", GetModuleHandle(NULL), SND_ASYNC);
	}

	void PlayShadowActivatedSound()
	{
		if (!m_SoundOn)
			return;
		// Source: Royalty-free Soniss GameAudioGDC packages
		PlaySound(L"Sound\\ShadowActivated.wav", GetModuleHandle(NULL), SND_ASYNC);
	}

	void PlayNextLevelSound()
	{
		if (!m_SoundOn)
			return;
		// Source: Royalty-free Soniss GameAudioGDC packages
		PlaySound(L"Sound\\NextLevel.wav", GetModuleHandle(NULL), SND_ASYNC);
	}

	void PlayLoseSound()
	{
		if (!m_SoundOn)
			return;
		StopCurrentTheme();
		// Source: Royalty-free Soniss GameAudioGDC packages
		PlaySound(L"Sound\\LoseGame.wav", GetModuleHandle(NULL), SND_ASYNC);
	}

	void PlayWinSound()
	{
		if (!m_SoundOn)
			return;

		StopCurrentTheme();
		// Source: Royalty-free Soniss GameAudioGDC packages
		PlaySound(L"Sound\\WinGame.wav", GetModuleHandle(NULL), SND_ASYNC);
	}
};
