#########################################################
##	template

FxInfo


Flags    InheritAlpha

#########################################################
####################### AUDIO ###########################
#########################################################

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

#########################################################
################## Tail Fin Orbs #######################
#########################################################

Condition
	On	Time
	Time	0

	Event
		EName	hookup
		Type	local
		At	Origin

		BhvrOverride
			Scale			.15 .12 .15
			PositionOffset		36 -1 0
			StartColor		200 55 255
		End

		Geom	BottemGlobe
	End

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		-163 -68 130
		End

		Part	:GlowingOrbs.part
	End

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		-163 -68 -130
		End

		Part	:GlowingOrbs.part
		
	End

#########################################################
################## Antenna Lights #######################
#########################################################

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		85.5 -46.5 -1
		End
		
		Part	:BlinkingLightCore.part
		Part	:BlinkingLights.part
	End

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		103.5 -41.5 -1
		End
		
		Part	:BlinkingLightCore.part
		Part	:BlinkingLights.part
	End


#########################################################
################## Top Antenna Lights #######################
#########################################################

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		-56.8 90 -1.7
		End
		
		Part	:BlinkingLightCore.part
		Part	:BlinkingLights.part
	End

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		-60 60 -2
		End
		
		Part	:BlinkingLightCore.part
		Part	:BlinkingLights.part
	End

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		-12 60.5 -2
		End
		
		Part	:BlinkingLightCore.part
		Part	:BlinkingLights.part
	End


#########################################################
################## Right Side Ports #######################
#########################################################

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PyrRotate		0 90 120	
			PositionOffset		58 -10 -26
		End
		
		part	:GlowingOrbiterCore.part
	End

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		58 -10 -26
		End
		
		Part	:GlowingOrbiterCenter.part
		Part	:GlowingOrbiterMist.part
	End

#########################################################

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PyrRotate		0 90 120	
			PositionOffset		38 -8 -26
		End
		
		part	:GlowingOrbiterCore.part
	End

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		38 -8 -26
		End
		
		Part	:GlowingOrbiterCenter.part
		Part	:GlowingOrbiterMist.part
	End

#########################################################

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PyrRotate		0 90 120	
			PositionOffset		17 -7 -26
		End
		
		part	:GlowingOrbiterCore.part
	End

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		17 -7 -26
		End
		
		Part	:GlowingOrbiterCenter.part
		Part	:GlowingOrbiterMist.part
	End

#########################################################
################## Left Side Ports #######################
#########################################################

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PyrRotate		0 80 70	
			PositionOffset		57 -10 29
		End
		
		part	:GlowingOrbiterCore.part
	End

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PyrRotate		0 110 50
			PositionOffset		58 -10 26
		End
		
		Part	:GlowingOrbiterCenter.part
		Part	:GlowingOrbiterMist.part
	End

#########################################################


	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PyrRotate		0 80 70	
			PositionOffset		37 -9 31
		End
		
		part	:GlowingOrbiterCore.part
	End

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PyrRotate		0 110 50
			PositionOffset		38 -9 26
		End
		
		Part	:GlowingOrbiterCenter.part
		Part	:GlowingOrbiterMist.part
	End

##########################################################


	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PyrRotate		0 80 70	
			PositionOffset		16 -8 32
		End
		
		part	:GlowingOrbiterCore.part
	End

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PyrRotate		0 120 30
			PositionOffset		17 -7 26
		End
		
		Part	:GlowingOrbiterCenter.part
		Part	:GlowingOrbiterMist.part
	End


#########################################################
################## MainJetEngine #######################
#########################################################

	Event
		Ename	Upwards
		Type	local
		At	Origin

		BhvrOverride
			PositionOffset		-40 10 0
		End
		
		Part	:EnergyElectricity.part
		Part	:EnergyElectricityInvert.part

		Part	:Blue_ThrusterCoreWhiteHottembers.part
		Part	:Blue_ThrusterCoreWhiteHottLicks.part
		Part	:Blue_ThrusterCoreFlames.part
		Part	:Blue_ThrusterCoreLiquid.part

#		Part	:Blue_ThrusterCoreLiquidDrips.part
#		Part	:Blue_ThrusterCore.part

		
	End

#########################################################
################## Right Side Jet Engines #######################
#########################################################

#	Event
#		Ename	Upwards
#		Type	local
#		At	Origin
#
#		BhvrOverride
#			PyrRotate		0 5 15
#			PositionOffset		-50 24 -37
#		End
#
#		Part	:Blue_SmallThrusterCoreWhiteHottLicks.part
#		Part	:Blue_SmallThrusterCoreWhiteHottLicks.part
#		Part	:Blue_SmallThrusterCore.part
#		Part	:Blue_SmallThrusterCoreFlames.part
#		
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
#		
#		Part	:Blue_SmallThrusterCoreWhiteHottLicks.part
#		Part	:Blue_SmallThrusterCoreWhiteHottLicks.part
#		Part	:Blue_SmallThrusterCore.part
#		Part	:Blue_SmallThrusterCoreFlames.part
#		
#	End
#
##########################################################
################### Left Side Jet Engines #######################
##########################################################
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
#		
#		Part	:Blue_SmallThrusterWhiteHottLicks.part		
#		Part	:Blue_SmallThrusterCoreWhiteHottLicks.part
#		Part	:Blue_SmallThrusterCore.part
#		Part	:Blue_SmallThrusterCoreFlames.part
#		
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
#		
#		Part	:Blue_SmallThrusterWhiteHottLicks.part
#		Part	:Blue_SmallThrusterCoreWhiteHottLicks.part
#		Part	:Blue_SmallThrusterCore.part
#		Part	:Blue_SmallThrusterCoreFlames.part
#		
#	End

End


End		