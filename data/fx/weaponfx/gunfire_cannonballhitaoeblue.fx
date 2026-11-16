#########################################################
##	onfire
##
FxInfo  

Lifespan 50

Input  
	InpName	Chest
End

Input  
	InpName	Root
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Hook
		Type	Local 
		At	root
		Geom	RootToChestAdjustment
		LifeSpan	8

	End

	Event
		EName 	Fire
		Type	Posit
		At	Chest
		
		Part	:SmokeExplosionblue.part
		Part1	:FireExplosion2blue.part
		LifeSpan	8
		
	End

	Event
		EName 	Sparks
		Type	Posit
		At	Chest
		Part1	:GernadeLauncherSparks2.part
		#Part2	:GernadeLauncherSpark.part
		Sound angelfireexplo 100.0 100.0 .97
		
	End

	Event
		EName 	RadiusFxScale
		Type	start 
		At	Hook
		altpiv	1
		bhvr    behaviors/powers/firecontrol/InfernobubbleA.bhvr
		Part	:FireBallHitRing1ablue.part
		Part	:FireBallHitRing2ablue.part
		Part	:FireBallHitRing3ablue.part
		Part	:InfernoExplosionblue.part
		LifeSpan	8
		
	End

End

Condition
	On	Time
	Time	0
		
	Repeat	5
	
	Event
		EName	Sparkup
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkOUT.fx
		
	End

End

Condition
	On	Time
	Time	0
		
	Repeat	3
	
	Event
		EName	Sparkout
		Type 	Start
		At	Chest
		BhvrOverride
			PositionOffset	0 0 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
		
	End

End

Condition
	On 	Time
	Time 	5

	Event
		EName 	Sparks
		Type	Destroy 
	End

End


End			