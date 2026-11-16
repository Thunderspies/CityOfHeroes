#########################################################
##	FireAura
FxInfo

Lifespan	300
###############################################################################
###################################### Audio #####################################
###############################################################################




Condition
	On 	Time
	Time	0

	Event
		Type 	Local
		At	Origin
		Sound snowangel2 100.0 100.0 .7
	End

End

Condition
	On 	Time
	Time	0

	Event
		Type 	Local
		At	Origin
		Sound rumble1 100.0 100.0 .5
	End

End






Condition
	On 	Time
	Time	60

	Event
		Type 	Local
		At	Origin
		Sound snowangel 100.0 100.0 .85
	End

End



#################################################################
############################# FX ########################################
#################################################################       #
									#
									#
									#
								       ###
								        #
Condition
	On 	time
	Time	60

	Event
		EName	headSparks
		Type	Local
		At	head
		Part4	:SnowFlakes.part
		lifeSpan	165

	End

	Event
		EName	wing
		Type	posit
		At	Root
		Part	:SnowAngelRaysSpread.part
		Part	:SnowAngelRaysSpread.part
		Part	:SnowAngelRaysSpreadFrost.part
		LifeSpan	35
	End

	Event
		EName	wing
		Type	posit
		At	Root

		Part	:SnowAngelRaysSpreadFlatFrost.part
		Part	:SnowAngelRaysSpreadFlat.part
		LifeSpan	40
	End
	
	Event
		EName	wing
		Type	posit
		At	Root

		part1	:SnowAngelRaysCoreFrost.part
		part1	:SnowAngelRaysCore.part
		LifeSpan	45
	End
End

Condition
	On 	time
	Time	60

	Event
		EName	wing
		Type	local
		At	Back	#chest	#winghook
		#AltPiv	1

#		part1	:SnowAngel01.part
		
		Anim	FX_wings_ANGEL		
		Bhvr	behaviors/FX_WingScale_Angel_Male.bhvr
		
		LifeSpan	54	#85
	End

#	Event
#		Type	Local
#		At	Back
#		
#		ChildFx	AnimatedCharacterParts/WingsFeatherFall.Fx
#	End
End

Condition
	On 	time
	Time	142	#82
	
	Event
		EName	wing
		Type	startPositOnly #posit
		At	chest

		Bhvr	Behaviors/GenericParticleFade.bhvr

		Part	:lightRays.part
		Part	:lightRaysLargeSpread.part
		
		
		LifeSpan	78
	End

	Event
		EName	headSparks
		Type	Local
		At	head
		Part	:IceMist.part
		lifeSpan	75

	End

End

Condition
	On 	Time
	Time 	60	#0

	Event
		EName	winghook
		Type	Local
		At	root
		Geom	RoottoHeadAdjustment
		Bhvr	Behaviors/PhoenixMoveUp.Bhvr
		
		BhvrOverride
			PositionOffset		0 3 0
		End

	End

	Event
		EName	LightRaysHook
		Type	Local
		At	root
		Geom	RoottoHeadAdjustment
		Bhvr	Behaviors/SnowAngelMoveUp.Bhvr
	End

	Event
		EName	Pivot
		Type	Local
		At	head
		
		part2	:FrostCore.part
		Lifespan	150

	End

	Event
		EName	Hips2
		Type	Local
		At	Hips
		
		part2	:FrostCore2.part
		Lifespan	150
	End

	Event
		EName	Chest
		Type	Local
		At	Chest
		
		part2	:FrostCore2.part
		Lifespan	150
	End
	
	Event
		EName	Lleg
		Type	Local
		At	UArmL
		part1	:FrostCore4.part
		Part	:IceMist.part
		Lifespan	150
	End

	Event
		EName	Rleg
		Type	Local
		At	UArmR
		part1	:FrostCore4.part
		Part	:IceMist.part
		Lifespan	150
	End

	Event
		EName	Lleg
		Type	Local
		At	LArmL
		part1	:FrostCore4.part
		Part	:IceMist.part
		Lifespan	150
	End

	Event
		EName	Rleg
		Type	Local
		At	LArmR
		part1	:FrostCore4.part
		Part	:IceMist.part
		Lifespan	150
	End

	Event
		EName	Lleg
		Type	Local
		At	UlegL
		part1	:FrostCore1.part
		Lifespan	150
	End

	Event
		EName	Rleg
		Type	Local
		At	UlegR
		part1	:FrostCore1.part
		Lifespan	150
	End

	Event
		EName	Lleg
		Type	Local
		At	LlegL
		part1	:FrostCore1.part
		Lifespan	150
	End

	Event
		EName	Rleg
		Type	Local
		At	LlegR
		part1	:FrostCore1.part
		Lifespan	150
	End

	Event
		EName	Lleg
		Type	Local
		At	FootL
		part1	:FrostCore1.part
		Lifespan	150
	End

	Event
		EName	Rleg
		Type	Local
		At	FootR
		part1	:FrostCore1.part
		Lifespan	150
	End
End

Condition
	On 	Time
	Time 	120	#60

	Event
		EName	InitialSwordFire
		Type	Local 
		At	WepR
		Part1	:FrostHandz.part

		Lifespan	100
	
	End


	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part1	:FrostHandz.part
	
		Lifespan	100
	End

End

Condition
	On	Time
	Time	155	#95

	Event
		EName	Hips
		Type	Local
		At	Hips
		part1	:FrostAura2.part
	End

End

Condition
	On	Time
	Time	180	#120

	Event
		EName	Hips
		Type	Local
		At	Hips
		part1	:FrostAura2.part
	End

End
		