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

#Condition
#	On 	time
#	Time 	0
#	Repeat 	24
#
#	Event 
#		EName 	Sparkzz
#		Type 	Local
#		At 	chest
#		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
#		Lifespan 60
#		Lifespanjitter 	20
#	End
#End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Fire
		Type	Posit
		At	Chest
		
		Part	:ProjectileSmokeExplosion.part
		Part1	:FireExplosion2.part
		LifeSpan	18
		
	End

	Event
		EName 	Sparks
		Type	Posit
		At	Chest
		Part1	:GernadeLauncherSparks2.part
		Sound bigboomexplo2 120.0 120.0 1.0
		Sound exploburst2 120.0 120.0 1.0
		Sound boom_explo4 1020.0 1020.0 1.0
		
	End

	Event
		EName 	TankBlast_flash
		Type	Posit
		At	Chest

		Part	:FireExplosionFlash.part
		Lifespan 5
		
	End

	Event
		EName 	RadiusFxScale
		Type	start 
		At	Chest  ##Hook
	
		bhvr    behaviors/powers/firecontrol/InfernobubbleA.bhvr
		Part	:FireBallHitRing2a.part
		Lifespan	10
	
	End

	Event	
		ENAME	ThunderShake1
		Type	Local
		At	Origin
		Bhvr	Behaviors/SSThunderClapShake.bhvr
		
		Lifespan	10
		
		
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