#############################################################
## PistolHit2.fx
#############################################################

FxInfo

LifeSpan 50

#############################################################

Input
	Inpname Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Prime
		Type 	Local
		At	Chest
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			StartJitter	0.5 1.0 0.5
		End
		Part1	:Cryonic_Hit_Mist_Large.part
		Part2	:Cryonic_Hit_Flash_Large.part
		Part	:Cryonic_Hit_Shockwave_Large.part
		Part	:Cryonic_Hit_Mist_Bright.part

		LifeSpan 10
	Lifespan 10

	End
End

#############################################################

End