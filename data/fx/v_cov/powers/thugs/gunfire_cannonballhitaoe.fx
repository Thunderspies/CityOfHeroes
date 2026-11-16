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
		
		Part	WeaponFX\SmokeExplosion.part
		Part1	WeaponFX\FireExplosion2.part
		LifeSpan	8
		
	End

	Event
		EName 	Sparks
		Type	Posit
		At	Chest
		Part1	WeaponFX\GernadeLauncherSparks2.part
		Sound grexplo2 120.0 120.0 1.0
		
	End

	Event
		EName 	RadiusFxScale
		Type	start 
		At	Chest
#		altpiv	1
		bhvr    behaviors/powers/firecontrol/InfernobubbleA.bhvr
		Part	powers/firecontrol/FireBallHitRing1a.part
#		Part	powers/firecontrol/FireBallHitRing2a.part
		Part	powers/firecontrol/FireBallHitRing3a.part
		Part	powers/firecontrol/InfernoExplosion.part
		LifeSpan	8
		
	End

End

Condition
	On	Time
	Time	0
		
	Repeat	7
	
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
		
	Repeat	11
	
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