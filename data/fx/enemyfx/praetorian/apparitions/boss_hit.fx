#############################################################
## Steam_High.fx
#############################################################

FxInfo
LifeSpan 30

#############################################################

Input
	InpName	Origin
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound 	Apparitions_Hit_01 100 100 .8
		Sound 	Apparitions_Hit_02 100 100 .8
		Sound 	Apparitions_Hit_03 100 100 .8
		Sound 	Apparitions_Hit_04 100 100 .8
	End

	Event
		Ename	RocketAnchor
		Type	Local
		At 	Root
		BhvrOverride
			PositionOffset	0.0 2.0 0.0
			StartJitter	3.0 2.0 3.0
		End
		Bhvr 	:RocketMotion.bhvr
		Lifespan 30
	End

	Event
		Ename 	Offset3
		Type 	Local
		At	RocketAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.0 0.0 1
		End
		Part	:Hit_Flash.part
		Part	:Hit_Smoke.part
		Part	:Hit_Smoke_Lingering.part

		Lifespan 5
End

#############################################################

End