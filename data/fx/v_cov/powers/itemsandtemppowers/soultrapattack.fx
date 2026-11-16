#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan	60

Condition
	On	Time
	Time	15

	Event
		EName	P
		Type	local
		At	wepR
		Bhvr	:SoulTrapObject.bhvr
		Geom	FX_MetalBall
		Sound soultrap1 80 80 .77
		LifeSpan	25

	End

	Event
		EName	pulse
		Type	SetLight
		At	P
		Bhvr	:SoulTrapPulse.bhvr
		Lifespan	120

	End

End

Condition
	On	Time
	Time	20

	Event
		EName	glow
		Type	local
		At	P
		Part	:SoulTrapGlow.part
		LifeSpan	60

	End

#	Event
#		EName	pulse
#		Type	SetLight
#		At	P
#		Bhvr	:SoulTrapPulse.bhvr
#		Lifespan	120
#
#	End
#
End

Condition
	On	Time
	Time	30

	Event
		Type	local
		At	P
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:AngelFireDeviceEnergy.part
		Part	:AngelFireCoreEnergy.part

		LifeSpan	25
	End

	Event
		Ename	rays
		Type	local
		At	T_Chest
		Part	:AngelFireLightRays.part
		Pmagnet	P
		LifeSpan	25
	End

End

End	


		