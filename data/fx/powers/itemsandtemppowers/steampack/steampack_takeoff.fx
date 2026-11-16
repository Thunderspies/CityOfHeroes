#############################################################
## SteamPack_Takeoff.fx
#############################################################

FxInfo
LifeSpan 80

#############################################################

Input
	InpName	Hips
End

#############################################################

Condition
	On 	Time
	Time	 0

	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound 	SP_SteamPack_Burst_01 100 100 .4
		Sound 	SP_SteamPack_Burst_02 100 100 .4
		Sound 	SP_SteamPack_Burst_03 100 100 .4
		Sound 	SP_SteamPack_Burst_04 100 100 .4
		Sound 	SP_SteamPack_Burst_05 100 100 .4
	End

	Event
		EName	Takeoff
		Type	Start
		At	Hips
		Bhvr	:Dust_Expanding.bhvr
		Part	:Takeoff_Dust.part
		Lifespan 80
	End
End

Condition
	On	Time
	Time	2

	Event
		EName	Takeoff
		Type	Start
		At	Root
		Bhvr	:Dust_Expanding.bhvr
		Part	:Takeoff_Dust_Lingering.part
		Lifespan 80
	End
End

#############################################################

End