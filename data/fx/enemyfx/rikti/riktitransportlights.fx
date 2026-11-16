#########################################################
## Rikti Transport Shuttle - Persistent FX
#########################################################

FxInfo

Flags    InheritAlpha

## SOUND FX #######################################################

#Condition
#
#	On 	Time	
#	Time	18
#
#	Event
#		Type Local
#		At origin
#		Sound	teletrans4 80 30 .88
#	End
#
#End
#
#Condition
#
#	On 	Time	
#	Time	70
#
#	Event
#		Type Local
#		At origin
#		Sound	teletrans3 80 30 .78
#	End
#
#End

## ANTANNAE LIGHTS #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		82.5 -95.5 -1
		End
		Part	:BlinkingLightCore.part
		Part	:BlinkingLights.part
	End

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		101.5 -90 0
		End
		Part	:BlinkingLightCore.part
		Part	:BlinkingLights.part
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		-59 42 -2
		End
		Part	:BlinkingLightCore.part
		Part	:BlinkingLights.part
	End

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		-63 12.5 -2
		End
		Part	:BlinkingLightCore.part
		Part	:BlinkingLights.part
	End

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		-14.5 13 -2
		End
		Part	:BlinkingLightCore.part
		Part	:BlinkingLights.part
	End
End

## TAIL FIN ORBS #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	Lights
		Type	local
		At	FootR
#		BhvrOverride
#			PositionOffset		-163 -68 130
#		End
		Part	:GlowingOrbs.part
	End

	Event
		Ename	Lights
		Type	local
		At	FootL
#		BhvrOverride
#			PositionOffset		-163 -68 -130
#		End
		Part	:GlowingOrbs.part
	End
End


## EXHAUST STARBOARD #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	Upwards
		Type	local
		At	WepR
		part	:GlowingOrbiterCore.part
		Part	:GlowingOrbiterCenter.part
		Part	:GlowingOrbiterMist.part
	End

	Event
		Ename	Upwards
		Type	local
		At	F1_R
		part	:GlowingOrbiterCore.part
		Part	:GlowingOrbiterCenter.part
		Part	:GlowingOrbiterMist.part
	End

	Event
		Ename	Upwards
		Type	local
		At	F2_R
		part	:GlowingOrbiterCore.part
		Part	:GlowingOrbiterCenter.part
		Part	:GlowingOrbiterMist.part
	End
End

## EXHAUSTS PORT #######################################################

Condition
	On	Time
	Time	0

	Event
		Ename	Upwards
		Type	local
		At	WepL
		part	:GlowingOrbiterCore.part
		Part	:GlowingOrbiterCenter.part
		Part	:GlowingOrbiterMist.part
	End

	Event
		Ename	Upwards
		Type	local
		At	F1_L
		part	:GlowingOrbiterCore.part
		Part	:GlowingOrbiterCenter.part
		Part	:GlowingOrbiterMist.part
	End

	Event
		Ename	Upwards
		Type	local
		At	F2_L
		part	:GlowingOrbiterCore.part
		Part	:GlowingOrbiterCenter.part
		Part	:GlowingOrbiterMist.part
	End
End


#########################################################
################## MainJetEngine #######################
#########################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		Ename	Upwards
#		Type	local
#		At	Origin
#
#		BhvrOverride
#			PositionOffset		-55 2 0
#		End
#		Part	:ThrusterCoreWhiteHottLicks.part
#		Part	:ThrusterCoreLiquidDrips.part
#		Part	:ThrusterCoreLiquid.part
#		Part	:ThrusterCore.part
#		Part	:ThrusterCoreFlames.part
#	End
#End


## STARBOARD ENGINE #######################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		Ename	Upwards
#		Type	local
#		At	Origin
#
#		BhvrOverride
#			PyrRotate		0 5 15
#			PositionOffset		-50 24 -37
#		End
#		Part	:SmallThrusterCoreWhiteHottLicks.part
#		Part	:SmallThrusterCore.part
#		Part	:SmallThrusterCoreFlames.part
#	End
#
#	Event
#		Ename	Upwards
#		Type	local
#		At	Origin
#
#		BhvrOverride
#			PyrRotate		0 5 -15
#			PositionOffset		-45 -6 -40
#		End
#		Part	:SmallThrusterCoreWhiteHottLicks.part
#		Part	:SmallThrusterCore.part
#		Part	:SmallThrusterCoreFlames.part
#	End
#End

## PORT ENGINE #######################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		Ename	Upwards
#		Type	local
#		At	Origin
#
#		BhvrOverride
#			PyrRotate		0 -5 15
#			PositionOffset		-50 24 37
#		End
#		Part	:SmallThrusterCoreWhiteHottLicks.part
#		Part	:SmallThrusterCore.part
#		Part	:SmallThrusterCoreFlames.part
#	End
#
#	Event
#		Ename	Upwards
#		Type	local
#		At	Origin
#
#		BhvrOverride
#			PyrRotate		0 -5 -15
#			PositionOffset		-45 -6 40
#		End
#		Part	:SmallThrusterCoreWhiteHottLicks.part
#		Part	:SmallThrusterCore.part
#		Part	:SmallThrusterCoreFlames.part
#	End
#End

#########################################################

End		