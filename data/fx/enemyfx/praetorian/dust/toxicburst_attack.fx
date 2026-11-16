#############################################################
## DronePlasmaBeam_Attack.fx
#############################################################

FxInfo
LifeSpan 90

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	10

	Event
		ENAME   GunOffset
		Type	Local
		At	WepR
		Geom	FlameThrower
		Sound DUST_ToxicBurst_Attack_01 100 100 0.5
	End
End

Condition
	On	Time
	Time	10

	Event
		Ename 	Prime
		Type	Local
		At	GunOffset
		AltPiv	1
		BhvrOverride
			PositionOffset	-1.0 0.0 0.0
		End
		Part	:Spray_Jet.part
		Part	:Spray_Body_Additive.part
		Part	:Spray_Body.part
	End
End

#############################################################

End