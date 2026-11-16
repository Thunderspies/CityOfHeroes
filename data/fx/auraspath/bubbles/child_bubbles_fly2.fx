#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 40

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.85 0.0
		End
		Part	:Toxic_Bubbles_Main_AIR.part
		Lifespan 	20
		LifespanJitter	15
	End
End

Condition
	On	Time
	Time	18

	Event
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset	0.0 1.0 0.0
		End
		Part	:Toxic_Splash_Flash.part
		Part	:Toxic_Splash_Main_AIR.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset	0.0 0.2 0.0
		End
		Part	:Toxic_Splash_Droplets_AIR.part
		Lifespan 20
	End
End
#############################################################

End