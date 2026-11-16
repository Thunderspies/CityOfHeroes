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

Input  
	InpName	Origin
End

Condition
	On 	time
	Time 	0
	Repeat 	24

	Event 
		EName 	Sparkzz
		Type 	start
		At 	chest
		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
		Lifespan 60
		Lifespanjitter 	20
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Hook
		Type	start 
		At	root
		Geom	RootToChestAdjustment

	End

	Event
		EName 	Fire
		Type	start
		At	Chest
		
		Part1	:FireExplosion2.part
		Lifespan	15
		
	End
	
	Event
		Type	start
		At	Chest
		
		Part	:SmokeExplosion.part
		Lifespan	25
		
	End


	Event
		EName 	Sparks
		Type	start
		At	Chest
		Part1	:GernadeLauncherSparks2.part
		Sound boom_explo3 100.0 100.0 .9
		
	End

	Event
		EName 	TankBlast_flash
		Type	start
		At	Chest

		Part	:FireExplosionFlash.part
		Lifespan 5
		
	End

	Event
		EName 	RadiusFxScale
		Type	start 
		At	Hook
		altpiv	1
		bhvr    behaviors/powers/firecontrol/InfernobubbleA.bhvr
		Part	:FireBallHitRing2a.part
		Lifespan	8
	
	End

	Event	
		ENAME	ThunderShake1
		Type	start
		At	Origin
		Bhvr	Behaviors/SSThunderClapShake.bhvr
		Lifespan	8
		
		
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