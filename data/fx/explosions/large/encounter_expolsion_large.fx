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
	On 	Time
	Time 	0

	Event
		EName 	Hook
		Type	Start
		At	root
		Geom	RootToChestAdjustment

	End

	Event
		EName 	Fire
		Type	Posit
		At	Mystic
		
		Part	:SmokeExplosion.part
		Part1	:FireExplosion2.part
		
	End

	Event
		EName 	Sparks
		Type	Posit
		At	Mystic
		Part1	:GernadeLauncherSparks2.part
		Sound boom_explo 100.0 100.0 .98
		
	End

	Event
		EName 	TankBlast_flash
		Type	Posit
		At	Mystic

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
	
	End

	Event	
		ENAME	ThunderShake1
		Type	Local
		At	Origin
		Bhvr	Behaviors/SSThunderClapShake.bhvr
		
		
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

Condition
	On 	Time
	Time 	8

	Event
		EName 	all
		Type	Destroy 
	End


End


End			