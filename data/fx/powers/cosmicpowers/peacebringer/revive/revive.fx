#############################################################
## Peacebringer - Restore Essence
#############################################################

FxInfo

LifeSpan 200

#############################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName 	Primea
#		Type	Local 
#		At	Eyes
#		BHVR	Behaviors/OffsetEyes.bhvr
#		Geom	CircleOfThorns
#		sound	peaceeyes1 60 50 .7
#		LifeSpan	200
#	End
#
#	Event
#		Type	Local 
#		At	Primea
#		 AltPiv 1
#		BHVR	Behaviors/GenericparticleFade.bhvr
#		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\CircleOfThornsEyes_21Green.part
#		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\EyesMistLeftBLUE.part
#	
#		LifeSpan	200
#	End
#	
#	Event
#		Type	Local 
#		At	Primea
#		 AltPiv 2
#		BHVR	Behaviors/GenericparticleFade.bhvr
#		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\CircleOfThornsEyes_21Green.part
#		Part	POWERS\CosmicPowers\PeaceBringer\ActivationEyes\EyesMistRightBLUE.part
#		LifeSpan	200
#	End
#End

#############################################################

Condition
	On	Time
	Time	10
	
	Event
		Type	local
		At	root
		part	:EnergyGlows.part
	End
	
	Event
		EName	Pivot
		Type	local
		At	root
		Geom	HotFeet
		Bhvr	:Scale.bhvr
		Sound flameburst_loop 60 60 .74
	End

	Event
		Ename	fireSound
		Type	Local	
		At	root
		Sound fireballfly 60 60 .3
		Geom	HotFeet01
		Bhvr	:Scale.bhvr
	End

	Event
		Ename	fireSound
		Type	start	
		At	root
		ChildFx	:TendrilBasechild.fx
		lifeSpan	200
	End
End

#############################################################

Condition
	On 	cycle
	Time 	20
	Chance	.4

	Event
		EName	Fire2
		Type	start
		At	firesound
		altpiv	2
		part	:Tendril.part
		part	:FlatTendrils.part
		Lifespan	9
	End
End

Condition
	On 	cycle
	Time 	24
	Chance	.2
	Event
		EName	Fire5
		Type	start
		At	firesound
		altpiv	5
		part	:Tendril.part
		part	:FlatTendrils.part
		Lifespan	10
	End
End

Condition
	On 	cycle
	Time 	21
	Chance	.4

	Event
		EName	Fire6
		Type	start
		At	firesound
		altpiv	6
		part	:Tendril.part
		part	:FlatTendrils.part
		Lifespan	8
	End
End

Condition
	On 	cycle
	Time 	25
	Chance	.4

	Event
		EName	Fire7
		Type	start
		At	firesound
		altpiv	7
		part	:Tendril.part
		part	:FlatTendrils.part
		Lifespan	9
	End
End

Condition
	On 	cycle
	Time 	28
	Chance	.4

	Event
		EName	Fire8
		Type	start
		At	firesound
		altpiv	8
		part	:Tendril.part
		part	:FlatTendrils.part
		Lifespan	10
	End
End

Condition
	On 	cycle
	Time 	22
	Chance	.4

	Event
		EName	Fire11
		Type	start
		At	firesound
		altpiv	12
		part	:Tendril.part
		part	:FlatTendrils.part
		Lifespan	8		
	End
End

Condition
	On 	cycle
	Time 	24
	Chance	.4

	Event
		EName	Fire11
		Type	start
		At	firesound
		altpiv	13
		part	:Tendril.part
		part	:FlatTendrils.part
		Lifespan	10		
	End
End

Condition
	On 	cycle
	Time 	22
	Chance	.4

	Event
		EName	Fire11
		Type	start
		At	firesound
		altpiv	14
		part	:Tendril.part
		part	:FlatTendrils.part
		Lifespan	9		
	End
End

#############################################################

Condition
	On 	cycle
	Time 	20
	Chance	.4

	Event
		EName	Fire2
		Type	start
		At	Pivot
		altpiv	2
		part	:Tendril.part
		part	:FlatTendrils.part
		Lifespan	9
	End
End

Condition
	On 	cycle
	Time 	24
	Chance	.4
	Event
		EName	Fire5
		Type	start
		At	Pivot
		altpiv	5
		part	:Tendril.part
		part	:FlatTendrils.part
		Lifespan	10
	End
End

Condition
	On 	cycle
	Time 	21
	Chance	.4

	Event
		EName	Fire6
		Type	start
		At	Pivot
		altpiv	6
		part	:Tendril.part
		part	:FlatTendrils.part
		Lifespan	8
	End
End

Condition
	On 	cycle
	Time 	25
	Chance	.4

	Event
		EName	Fire7
		Type	start
		At	Pivot
		altpiv	7
		part	:Tendril.part
		part	:FlatTendrils.part
		Lifespan	9
	End
End

Condition
	On 	cycle
	Time 	26
	Chance	.4

	Event
		EName	Fire8
		Type	start
		At	Pivot
		altpiv	8
		part	:Tendril.part
		part	:FlatTendrils.part
		Lifespan	10
	End
End

Condition
	On 	cycle
	Time 	22
	Chance	.4

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	12
		part	:Tendril.part
		part	:FlatTendrils.part
		Lifespan	8		
	End
End

Condition
	On 	cycle
	Time 	24
	Chance	.4

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	13
		part	:Tendril.part
		part	:FlatTendrils.part
		Lifespan	10		
	End
End

Condition
	On 	cycle
	Time 	22
	Chance	.4

	Event
		EName	Fire11
		Type	start
		At	Pivot
		altpiv	14
		part	:Tendril.part
		part	:FlatTendrils.part
		Lifespan	9		
	End
End

#############################################################

Condition

	On	time
	Time	60

	Event
		EName	Chest
		Type	Local
		At	Col_L
		part1	:PeaceCoreEnergyHead.part
	End

	Event
		EName	Chest
		Type	Local
		At	Col_R
		part1	:PeaceCoreEnergyHead.part
	End

	Event
		EName	Lleg
		Type	Local
		At	LlegL
		part1	:PeaceCore1.part
		part2	:PeaceCoreEnergy2.part
		magnet	UlegL
	End

	Event
		EName	Rleg
		Type	Local
		At	LlegR
		part1	:PeaceCore1.part
		part2	:PeaceCoreEnergy3.part
		magnet	UlegL
	End

	Event
		EName	Lleg
		Type	Local
		At	UlegL
		part1	:PeaceCore.part
		part2	:PeaceCoreEnergy3.part
	End

	Event
		EName	Rleg
		Type	Local
		At	UlegR
		part1	:PeaceCore.part
		part2	:PeaceCoreEnergy2.part
	End

	Event
		EName	Lleg
		Type	Local
		At	FootL
		part1	:PeaceCore1.part
		part2	:PeaceCoreEnergy2.part
		magnet	LlegL
	End

	Event
		EName	Rleg
		Type	Local
		At	FootR
		part1	:PeaceCore1.part
		part2	:PeaceCoreEnergy3.part
		magnet	LlegR
	End
	
	Event
		EName	Larm
		Type	Local
		At	UarmL
		part1	:ArmPeaceCore1.part
		part2	:ArmPeaceCoreEnergy2.part
		magnet	LarmL
	End

	Event
		EName	Rarm
		Type	Local
		At	UarmR
		part1	:ArmPeaceCore1.part
		part2	:ArmPeaceCoreEnergy2.part
		magnet	LarmR
	End

	Event
		EName	Larm
		Type	Local
		At	LarmL
		part1	:ArmPeaceCore1.part
		part2	:ArmPeaceCoreEnergy2.part
		magnet	wepL
	End

	Event
		EName	Rarm
		Type	Local
		At	LarmR
		part1	:ArmPeaceCore1.part
		part2	:ArmPeaceCoreEnergy2.part
		magnet	wepR
	End

	Event
		EName	Larm
		Type	Local
		At	wepL
		part1	:ArmPeaceCore1.part
		part2	:ArmPeaceCoreEnergy2.part
		magnet	LarmL
	End

	Event
		EName	Rarm
		Type	Local
		At	wepR
		part1	:ArmPeaceCore1.part
		part2	:ArmPeaceCoreEnergy2.part
		magnet	LarmR
	End
End

#############################################################

Condition

	On	Time
	Time	120

	Event
		EName	Rarm
		Type	Local
		At	chest
		part	:tendrilLightRays.part
		magnet	LarmR
	End
End

#############################################################

End				