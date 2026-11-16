#############################################################
## PsionicShockwave_Attack.fx
#############################################################

FxInfo
Lifespan 80

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
	#	Sound 	FS_PsychicLash_Hit_01 100 100 .8
	End

	Event
		Type	Local
		At	Head
		Part	:Head_Flash.part
		Lifespan	10
	End

End

Condition
	On	Time
	Time	1

	Event
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset	0.0 -2.0 0.0
		End
		Part	:Hit_Mouth.part
		Lifespan	10
	End

	Event
		Type	Local
		At	Head
		Part	:PsionicSnap_Electricity.part
		Part	:PsionicSnap_Smoke.part
		Lifespan	12
	End
End

#############################################################

End