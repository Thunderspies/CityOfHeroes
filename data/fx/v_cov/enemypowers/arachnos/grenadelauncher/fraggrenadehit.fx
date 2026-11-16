#########################################################
##	onfire
##
FxInfo  

Input  
	InpName	hips
End

Input  
	Inpname	chest
End

Input  
	Inpname	root
End

LifeSpan	100

Condition
	On 	Time
	Time 	0
	
	Event
		Type	Start
		At	T_Root
		BHVR	Behaviors/CameraShake01Subtle.bhvr
	End

	Event
		EName 	Hook
		Type	Local 
		At	root
		Geom	RootToChestAdjustment
		Sound fragGrenade 80 80 .9
	End

	Event
		EName 	Fire
		Type	Posit
		At	chest
		Part1	powers/firecontrol/FireBallTargetSteam.part
		Part2	powers/firecontrol/FireBallTargetFlame2.part
		Part3	powers/firecontrol/FireBallSpark.part	
		
		LifeSpan	50
			
	End


	Event
		EName 	Prime1
		Type	Local 
		At	root
		
		Part1	Generic\InfernoCare.part
		Part2	Generic\Inferno.part
		

	End

	Event
		EName 	Prime2
		Type	Local 
		At	root
		Part1	powers/firecontrol/InfernoExplosion.part
		

	End

	Event
		EName 	RadiusFxScale
		Type	start 
		At	Hook
		altpiv	1
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		#Part1	powers/firecontrol/FireBallHitRing1.part
		#Part2	powers/firecontrol/FireBallHitRing2.part
		
	End

End

Condition
	On 	Time
	Time 	3

	Event
		EName 	Prime
		Type	Local 
		At	root
		Part1	Generic\InfernorockBits.part
		Part2	Generic\InfernorockBitz.part
		#Sound	FireExplo 100.0 30.0 .8	
	End

End

Condition
	Time 	3
	Chance	.8

	Event
		Type	start
		At	hips

		Bhvr	Behaviors\StartOffset.bhvr
		BhvrOverride
			PositionOffset	0 1 0
			Drag			0.003
			physics 		1
			physRadius 		0.3
			physGravity 		.75
			physRestitution 	0.25
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 	.001
		CDestroy 	0
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	75
		LifespanJitter	15
	End

End

Condition
	Time 	3
	Chance	.8

	Event
		Type	start
		At	hips

		Bhvr	Behaviors\StartOffset2.bhvr
		BhvrOverride
			PositionOffset	0 1 0
			Drag			0.003
			physics 		1
			physRadius 		0.3
			physGravity 		.75
			physRestitution 	0.25
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 	.001
		CDestroy 	0
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	75
		LifespanJitter	15
	End

End

Condition
	Time 	3
	Chance	.8

	Event
		Type	start
		At	hips

		Bhvr	Behaviors\StartOffset3.bhvr
		BhvrOverride
			PositionOffset	0 1 0
			Drag			0.003
			physics 		1
			physRadius 		0.3
			physGravity 		.75
			physRestitution 	0.25
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 	.001
		CDestroy 	0
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	75
		LifespanJitter	15
	End

End

Condition
	Time 	3
	Chance	.8

	Event
		Type	start
		At	hips

		Bhvr	Behaviors\StartOffset4.bhvr
		BhvrOverride
			PositionOffset	0 1 0
			Drag			0.003
			physics 		1
			physRadius 		0.3
			physGravity 		.75
			physRestitution 	0.25
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 	.001
		CDestroy 	0
		Part1	powers/firecontrol/fire.part
		Part1	powers/firecontrol/fire.part
		Lifespan	75
		LifespanJitter	15
	End

End

Condition
	On 	Time
	Time 	0
		
	Repeat	24
	
	Event
		EName	Sparkout
		Type 	Start
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
		
	End

End

Condition
	On 	Time
	Time	9

	Event
		EName 	Prime1
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	3

	Event
		EName 	Prime2
		Type	Destroy
	End

End


Condition
	On 	Time
	Time	7

	Event
		EName 	Prime
		Type	Destroy
	End

End	

Condition
	On 	Time
	Time 	30

	Event
		EName 	RadiusFxScale
		Type	Destroy 
	End


End

Condition
	On 	Time
	Time 	120

	Event
		EName 	All
		Type	Destroy 
	End


End


End			