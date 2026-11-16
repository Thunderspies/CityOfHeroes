#########################################################
##	template

FxInfo


Lifespan	140

###########################################################
################## Audio ##############################
###########################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	Local
#		At	Origin
#		Sound	clockworkmaterialize2 70 60 .76
#		
#	End
#
#End

###########################################################
################## Explosion ##############################
###########################################################

Condition
	On 	Time
	Time 	75

	Event
		ENAME	ChestXplode3
		Type	Posit
		At	ChestNode
		Part	EnemyFX/Clockwork/ElectricityExplosionSmall.part
		LifeSpan 15
		
	End

End

Condition
	On 	Time
	Time 	90

	Event
		ENAME	ChestXplode3
		Type	Posit
		At	ChestNode
		Part	EnemyFX/Clockwork/ElectricityExplosionBig.part
		LifeSpan 20
		
	End

End

###########################################################
################## Animation ##############################
###########################################################

Condition
	On 	Time
	Time 	1

	Event
		Ename	AnimationNode
		Type	Start
		At	Root
		Anim	FX_skeleton_spawn
		Bhvr	EnemyFX/Clockwork/GeoFadeUp.bhvr
		
		LifeSpan 100
	End
End

###########################################################
##################    FX     ##############################
###########################################################

#Condition
#	On 	Time
#	Time 	5
#
#	Event
#		ENAME	HipsNode
#		Type	Local
#		At	AnimationNode
#		Part	EnemyFX/Clockwork/ElectricityGlow01.part
#		AnimPiv Hips	
#	End
#
#	Event
#		ENAME	ChestNode
#		Type	Local
#		At	AnimationNode
#		Part	EnemyFX/Clockwork/ElectricityGlow02.part
#		AnimPiv Chest	
#	End
#
#	Event
#		ENAME	HeadNode
#		Type	Local
#		At	AnimationNode
#		Part	EnemyFX/Clockwork/ElectricityGlow01.part
#		AnimPiv Head	
#	End
#
#	Event
#		ENAME	LarmRNode
#		Type	Local
#		At	AnimationNode
#		Part	EnemyFX/Clockwork/ElectricityGlow02.part
#		AnimPiv LarmR	
#	End
#
#	Event
#		ENAME	UarmLNode
#		Type	Local
#		At	AnimationNode
#		Part	EnemyFX/Clockwork/ElectricityGlow01.part
#		AnimPiv UarmL	
#	End
#
#	Event
#		ENAME	LlegRNode
#		Type	Local
#		At	AnimationNode
#		Part	EnemyFX/Clockwork/ElectricityGlow02.part
#		AnimPiv LlegR	
#	End
#
#	Event
#		ENAME	LlegLNode
#		Type	Local
#		At	AnimationNode
#		Part	EnemyFX/Clockwork/ElectricityGlow01.part
#		AnimPiv LlegL	
#	End
#
#End

###########################################################
##################   Chest FX     ##########################
###########################################################

Condition
	On 	Cycle
	Time 	10

	Event
		ENAME	ChestFX1
		Type	Local
		At	AnimationNode
		AnimPiv	Chest
		Part	EnemyFX/Clockwork/ElectricityBolt01.part
		Part	EnemyFX/Clockwork/ElectricityBoltGlow01.part
		POther	HipsNode
		LifeSpan 5
		
	End

End

Condition
	On 	Cycle
	Time 	11

	Event
		ENAME	ChestFX2
		Type	Local
		At	AnimationNode
		AnimPiv	Chest
		Part	EnemyFX/Clockwork/ElectricityBolt01.part
		Part	EnemyFX/Clockwork/ElectricityBoltGlow01.part
		POther	HeadNode
		LifeSpan 5
		
	End

End

Condition
	On 	Cycle
	Time 	9
	Event
		ENAME	ChestFX3
		Type	Local
		At	AnimationNode
		AnimPiv	Chest
		Part	EnemyFX/Clockwork/ElectricityBolt01.part
		Part	EnemyFX/Clockwork/ElectricityBoltGlow01.part
		POther	LarmRNode
		LifeSpan 5
		
	End

End

Condition
	On 	Cycle
	Time 	12

	Event
		ENAME	ChestFX4
		Type	Local
		At	AnimationNode
		AnimPiv	Chest
		Part	EnemyFX/Clockwork/ElectricityBolt01.part
		Part	EnemyFX/Clockwork/ElectricityBoltGlow01.part
		POther	UarmLNode
		LifeSpan 5
		
	End

End

Condition
	On 	Cycle
	Time 	13

	Event
		ENAME	ChestFX5
		Type	Local
		At	AnimationNode
		AnimPiv	Chest
		Part	EnemyFX/Clockwork/ElectricityBolt01.part
		Part	EnemyFX/Clockwork/ElectricityBoltGlow01.part
		POther	LlegRNode
		LifeSpan 5
		
	End

End

Condition
	On 	Cycle
	Time 	10

	Event
		ENAME	ChestFX6
		Type	Local
		At	AnimationNode
		AnimPiv	Chest
		Part	EnemyFX/Clockwork/ElectricityBolt01.part
		Part	EnemyFX/Clockwork/ElectricityBoltGlow01.part
		POther	LlegLNode
		LifeSpan 15
		
	End

End

###########################################################
##################   Root FX     ##########################
###########################################################

Condition
	On 	Cycle
	Time 	20

	Event
		ENAME	ChestFX1
		Type	Local
		At	Root
		Part	EnemyFX/Clockwork/ElectricityBolt01.part
		Part	EnemyFX/Clockwork/ElectricityBoltGlow01.part
		POther	HipsNode
		LifeSpan 5
		
	End

End

Condition
	On 	Cycle
	Time 	21

	Event
		ENAME	ChestFX2
		Type	Local
		At	Root
		Part	EnemyFX/Clockwork/ElectricityBolt01.part
		Part	EnemyFX/Clockwork/ElectricityBoltGlow01.part
		POther	HeadNode
		LifeSpan 5
		
	End

End

Condition
	On 	Cycle
	Time 	19
	Event
		ENAME	ChestFX3
		Type	Local
		At	Root
		Part	EnemyFX/Clockwork/ElectricityBolt01.part
		Part	EnemyFX/Clockwork/ElectricityBoltGlow01.part
		POther	LarmRNode
		LifeSpan 5
		
	End

End

Condition
	On 	Cycle
	Time 	22

	Event
		ENAME	ChestFX4
		Type	Local
		At	Root
		Part	EnemyFX/Clockwork/ElectricityBolt01.part
		Part	EnemyFX/Clockwork/ElectricityBoltGlow01.part
		POther	UarmLNode
		LifeSpan 5
		
	End

End

Condition
	On 	Cycle
	Time 	23

	Event
		ENAME	ChestFX5
		Type	Local
		At	Root
		Part	EnemyFX/Clockwork/ElectricityBolt01.part
		Part	EnemyFX/Clockwork/ElectricityBoltGlow01.part
		POther	LlegRNode
		LifeSpan 5
		
	End

End

Condition
	On 	Cycle
	Time 	20

	Event
		ENAME	ChestFX6
		Type	Local
		At	Root
		Part	EnemyFX/Clockwork/ElectricityBolt01.part
		Part	EnemyFX/Clockwork/ElectricityBoltGlow01.part
		POther	LlegLNode
		LifeSpan 15
		
	End

End

###########################################################
##################   Head FX     ##########################
###########################################################

Condition
	On 	Cycle
	Time 	10

	Event
		ENAME	HeadFX1
		Type	Local
		At	AnimationNode
		AnimPiv	Head
		Part	EnemyFX/Clockwork/ElectricityBolt01.part
		Part	EnemyFX/Clockwork/ElectricityBoltGlow01.part
		POther	HipsNode
		LifeSpan 5
		
	End

End


Condition
	On 	Cycle
	Time 	15

	Event
		ENAME	HeadFX3
		Type	Local
		At	AnimationNode
		AnimPiv	Head
		Part	EnemyFX/Clockwork/ElectricityBolt01.part
		Part	EnemyFX/Clockwork/ElectricityBoltGlow01.part
		POther	LarmRNode
		LifeSpan 5
		
	End

End

Condition
	On 	Cycle
	Time 	20

	Event
		ENAME	HeadFX4
		Type	Local
		At	AnimationNode
		AnimPiv	Head
		Part	EnemyFX/Clockwork/ElectricityBolt01.part
		Part	EnemyFX/Clockwork/ElectricityBoltGlow01.part
		POther	UarmLNode
		LifeSpan 5
		
	End

End

Condition
	On 	Cycle
	Time 	25

	Event
		ENAME	HeadFX5
		Type	Local
		At	AnimationNode
		AnimPiv	Head
		Part	EnemyFX/Clockwork/ElectricityBolt01.part
		Part	EnemyFX/Clockwork/ElectricityBoltGlow01.part
		POther	LlegRNode
		LifeSpan 5
		
	End

End

Condition
	On 	Cycle
	Time 	30

	Event
		ENAME	HeadFX6
		Type	Local
		At	AnimationNode
		AnimPiv	Head
		Part	EnemyFX/Clockwork/ElectricityBolt01.part
		Part	EnemyFX/Clockwork/ElectricityBoltGlow01.part
		POther	LlegLNode
		LifeSpan 5
		
	End

End

###########################################################
##################   UarmL FX     #########################
###########################################################

Condition
	On 	Cycle
	Time 	10

	Event
		ENAME	UarmLFX1
		Type	Local
		At	AnimationNode
		AnimPiv	UarmL
		Part	EnemyFX/Clockwork/ElectricityBolt01.part
		Part	EnemyFX/Clockwork/ElectricityBoltGlow01.part
		POther	HipsNode
		LifeSpan 5
		
	End

End


Condition
	On 	Cycle
	Time 	15

	Event
		ENAME	UarmLFX3
		Type	Local
		At	AnimationNode
		AnimPiv	UarmL
		Part	EnemyFX/Clockwork/ElectricityBolt01.part
		Part	EnemyFX/Clockwork/ElectricityBoltGlow01.part
		POther	LarmRNode
		LifeSpan 5
		
	End

End

Condition
	On 	Cycle
	Time 	20

	Event
		ENAME	UarmLFX4
		Type	Local
		At	AnimationNode
		AnimPiv	UarmL
		Part	EnemyFX/Clockwork/ElectricityBolt01.part
		Part	EnemyFX/Clockwork/ElectricityBoltGlow01.part
		POther	UarmLNode
		LifeSpan 5
		
	End

End

Condition
	On 	Cycle
	Time 	25

	Event
		ENAME	UarmLFX5
		Type	Local
		At	AnimationNode
		AnimPiv	UarmL
		Part	EnemyFX/Clockwork/ElectricityBolt01.part
		Part	EnemyFX/Clockwork/ElectricityBoltGlow01.part
		POther	LlegRNode
		LifeSpan 5
		
	End

End

Condition
	On 	Cycle
	Time 	30

	Event
		ENAME	UarmLFX6
		Type	Local
		At	AnimationNode
		AnimPiv	UarmL
		Part	EnemyFX/Clockwork/ElectricityBolt01.part
		Part	EnemyFX/Clockwork/ElectricityBoltGlow01.part
		POther	LlegLNode
		LifeSpan 5
		
	End

End

###########################################################
##################   Explosion    #########################
###########################################################

Condition
	On 	Time
	Time 	10

	Event
		ENAME	ChestXplode
		Type	Local
		At	AnimationNode
		AnimPiv	Chest
		Part	EnemyFX/Clockwork/ElectricityExplosion01.part
		
	End

End

Condition
	On 	Time
	Time 	30

	Event
		ENAME	ChestXplode2
		Type	Start
		At	Root
		Part	EnemyFX/Clockwork/ElectricityExplosion02.part
		Part	EnemyFX/Clockwork/ElectricityExplosion01.part
		#Part	EnemyFX/Clockwork/ElectricityGroundGlow.part
		
	End

End



#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	100
	

	Event	
		ENAME	ChestGlow
		Type	Local
		At	Hips
		part1	EnemyFX/Clockwork/ElectricityBodyGlow.part
		Part	EnemyFX/Clockwork/ElectricityGlow01.part
		POther Neck
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
	
	End

	Event	
		ENAME	HeadGlow
		Type	Local
		At	Hair
		part1	EnemyFX/Clockwork/ElectricityBodyGlow.part
		Part	EnemyFX/Clockwork/ElectricityGlow01.part
		POther Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End
	

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part1	EnemyFX/Clockwork/ElectricityBodyGlow.part
		Part	EnemyFX/Clockwork/ElectricityGlow01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	
	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	WepR
		part1	EnemyFX/Clockwork/ElectricityBodyGlow.part
		Part	EnemyFX/Clockwork/ElectricityGlow01.part
		POther LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part1	EnemyFX/Clockwork/ElectricityBodyGlow.part
		Part	EnemyFX/Clockwork/ElectricityGlow01.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End
	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	WepL
		part1	EnemyFX/Clockwork/ElectricityBodyGlow.part
		Part	EnemyFX/Clockwork/ElectricityGlow01.part
		POther LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


	Event	
		ENAME	LeftULegGlow
		Type	Local
		At	LlegL
		part1	EnemyFX/Clockwork/ElectricityBodyGlow.part
		Part	EnemyFX/Clockwork/ElectricityGlow01.part
		POther Ulegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		part1	EnemyFX/Clockwork/ElectricityBodyGlow.part
		Part	EnemyFX/Clockwork/ElectricityGlow01.part
		POther Llegl
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightULegGlow
		Type	Local
		At	LlegR
		part1	EnemyFX/Clockwork/ElectricityBodyGlow.part
		Part	EnemyFX/Clockwork/ElectricityGlow01.part
		POther UlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End

	Event	
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		part1	EnemyFX/Clockwork/ElectricityBodyGlow.part
		Part	EnemyFX/Clockwork/ElectricityGlow01.part
		POther LlegR
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
		
	End


End


#Condition
#	On 	Time
#	Time 	90
#
#	Event
#		ENAME	All
#		Type	Destroy
#	End
#
#End

End		