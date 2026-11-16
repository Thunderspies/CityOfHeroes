############################################################
##	Fireweapon
###########################################################

FxInfo
Lifespan	80

Flags	InheritAnimScale

#################################################################################

Condition
	On 	Time
	Time	12
	Event
		Type	Local
		At	Root
		Sound 	BPB_FrigidQuills_Attack_01 200 100 .8
	End
End

	Condition
		On Time
		Time 10


	Event
		EName 	Smoke02
		Type	Local
		At	Fore_FootR
		Part	:FrigidSlash_Claws_Smoke.part
		Part	:FrigidSlash_Claws_Subtractive.part
		POther	Fore_ToeR
		Lifespan	50
	End

	Event
		EName 	Smoke01
		Type	Local
		At	Fore_ToeR
		Part	:FrigidSlash_Claws_Glow_Additive.part
		Part	:FrigidSlash_Claws_Glow.part
		POther	Fore_FootR
		Lifespan	40
	End

	Event
		EName 	Smoke01
		Type	Local
		At	Fore_LLegR
		Part	:FrigidSlash_Claws_Subtractive_Light.part
		POther	Fore_FootR
		Lifespan	50
	End

End

Condition
	On	Time
	Time	15

	Event
		EName 	Smoke01
		Type	Local
		At	Fore_FootL
		Part	:FrigidSlash_Claws_Smoke.part
		Part	:FrigidSlash_Claws_Subtractive.part
		POther	Fore_ToeL
		Lifespan	50
	End

	Event
		EName 	Smoke02
		Type	Local
		At	Fore_ToeL
		Part	:FrigidSlash_Claws_Glow_Additive.part
		Part	:FrigidSlash_Claws_Glow.part
		POther	Fore_FootL
		Lifespan	40
	End

	Event
		EName 	Smoke02
		Type	Local
		At	Fore_LLegL
		Part	:FrigidSlash_Claws_Subtractive_Light.part
		POther	Fore_FootL
		Lifespan	50

	End
End

Condition
	On	Time
	Time	41

	Event
		EName	SpineAnchor
		Type	Local
		At	Hips
		Part	:FrigidQuills_Flash.part
		Part	:FrigidQuills_Boils.part
		Part	:Hit_Shockwave_Drone.part
		Part	:FrigidQuills_Smoke.part
		Lifespan 15
	End
End

Condition
	On	Time
	Time	43

	Event
		EName	ElectricityAnchor
		Type	Local
		At	Hips
		Part	:FrigidQuills_Particles.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	43
	Repeat 	10

		Event
			At 	Hips
			Type 	Local
			CHILDFX	:CHILD_FrigidQuills_Attack.fx
			LifeSpan 	30
		End
End

Condition
	On	Cycle
	Time	2

		Event
			At 	SpineAnchor
			Type 	Local
			CHILDFX	:CHILD_FrigidQuills_Attack.fx
			LifeSpan 	10
		End
End

#################################################################################

End