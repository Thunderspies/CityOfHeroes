#########################################################
##	
########################################################
FxInfo


Flags    InheritAlpha

##################################################################

Condition
	On	Time
	Time	0
	Event
		EName 	Prop
		Type	Local
		At	Mystic
		Anim	FX_Eagle_Skiff
		Sound skiff1_loop 120 120 .69

	End

End


#####  kill skiff FX #####################################

Condition
	On 		triggerbits
	Triggerbits	death

	Event
		EName 	Prop
		Type	Destroy
	
	End

	Event
		EName 	partz
		Type	Start
		At	Root
		ChildFX	Explosions\VehicleExplosion\Expolsion_Skiff_Longbow.fx
		
	End


	Event
		EName 	Fire
		Type	Start
		At	BendMystic	
		Part	Explosions\Large\SmokeExplosion.part
		Part1	Explosions\Large\FireExplosion2.part
		Lifespan 5
		
	End

	Event
		EName 	Sparks
		Type	Start
		At	BendMystic
		Part1	Explosions\Large\GernadeLauncherSparks2.part
		Sound genexplo1 120.0 120.0 1.0
		Lifespan 5
		
	End

	Event
		EName 	TankBlast_flash
		Type	Start
		At	BendMystic

		Part	Explosions\Large\FireExplosionFlash.part
		Lifespan 5
		
	End

	Event
		EName 	RadiusFxScale
		Type	start 
		At	BendMystic
		bhvr    behaviors/powers/firecontrol/InfernobubbleA.bhvr
		Part	Explosions\Large\FireBallHitRing2a.part
		Lifespan 8
	
	End

	Event	
		ENAME	ThunderShake1
		Type	Local
		At	Origin
		Bhvr	Behaviors/SSThunderClapShake.bhvr
		Lifespan 8
		
		
	End


End




End


