
#########################################################################################

FxInfo
Flags	InheritAnimScale

Lifespan	80

#################################################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BPD_FrigidThorns_VOX_01 100 100 .8
	End
End

Condition
	On 	Time
	Time	15
	Event
		Type	Local
		At	Root
		Sound 	BPD_FrigidThorns_Attack_01 200 100 .8
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Smoke01
		Type	Local
		At	HandL
		BhvrOverride
			PositionOffset	-2.0 0.0 0.0
		End
		Part	:Defiler_FrigidSlash_Claws_SmokeL.part
		Part	:Defiler_FrigidSlash_Claws_SubtractiveL.part
		POther	HandL
		Lifespan 70
	End


	Event
		Type	Local
		At	HandR
		BhvrOverride
			PositionOffset	2.0 0.0 0.0
		End
		Part	:Defiler_FrigidSlash_Claws_Smoke.part
		Part	:Defiler_FrigidSlash_Claws_Subtractive.part
		POther	HandR
		Lifespan 70
	End
End


Condition
	On	Time
	Time	45
	Repeat	18

		Event
			At 	Chest
			Type 	Local
			CHILDFX	:CHILD_FrigidThorns_Attack.fx
			LifeSpan 	12
		End
End

Condition
	On	Time
	Time	45

		Event
			EName	SpineAnchor
			Type	Local
			At	Chest
			Part	:Hit_Shockwave_Main.part
			Part	:FrigidThorns_Flash.part
			Part	:FrigidQuills_Boils_Large.part
			Part	:Hit_Shockwave_Large.part
			Part	:FrigidQuills_Smoke.part
			Lifespan 30
		End
End

Condition
	On	Time
	Time	50

	Event
		EName	ElectricityAnchor
		Type	Local
		At	HandL
		Part	:Defiler_FrigidQuills_ParticlesL.part
		Lifespan 15
	End

	Event
		EName	ElectricityAnchor
		Type	Local
		At	HandR
		Part	:Defiler_FrigidQuills_Particles.part
		Lifespan 15
	End

End

#################################################################################

End