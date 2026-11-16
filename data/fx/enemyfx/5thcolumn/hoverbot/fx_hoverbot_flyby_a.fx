
FxInfo

Flags    InheritAlpha

LifeSpan 100


##### first bot flyby  ################################

Condition
	On 	Time
	Time 	0


	Event
		EName	1st_bot
		Type	Local
		At	BendMystic
		Anim	FX_hoverbot_flyby
		Bhvr	Behaviors/fadein.bhvr
		LifeSpan 26
	End

End


#######  1st explode  #####################################


Condition
	On 	Time
	Time 	26


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
		Sound grexplo2 120.0 120.0 1.0
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

#	Event	
#		ENAME	ThunderShake1
#		Type	Local
#		At	Origin
#		Bhvr	Behaviors/SSThunderClapShake.bhvr
#		Lifespan 8
#		
#		
#	End


End


########  second bot flyby  ################################

Condition
	On 	Time
	Time 	32


	Event
		EName	2nd_bot
		Type	Local
		At	BendMystic
		Anim	FX_hoverbot_flyby
		Bhvr	Behaviors/fadein.bhvr
		LifeSpan 26
	End

End


#######  2nd explode  #####################################


Condition
	On 	Time
	Time 	58


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
		Sound grexplo2 120.0 120.0 1.0
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

#	Event	
#		ENAME	ThunderShake1
#		Type	Local
#		At	Origin
#		Bhvr	Behaviors/SSThunderClapShake.bhvr
#		Lifespan 8
#		
#		
#	End


End

######  Skiff shooting  ####



End