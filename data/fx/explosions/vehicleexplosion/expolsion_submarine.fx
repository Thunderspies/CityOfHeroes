#########################################################
##	car on fire
#########################################################

FxInfo  

Lifespan 1400

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	chest
		
#		Sound	flierexplo1 200 30 .93

	End

End


########   Explosion Force   #########################

Condition
	On	Time
	Time	0

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Chest
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		100
			PhysForcePower		300
			PhysForcePowerJitter	50
		End

		Lifespan	1

	End

End

########  Main Explosion Particles   #########################

Condition

	Event
		EName 	MainFireBurst
		Type	local
		At	Chest

#		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0 1 -65
		End
		
	End
	
#	Event
#		EName 	Ripples
#		Type	local
#		At	Chest
#		BhvrOverride
#			PositionOffset	0 6 -65
#		End
#		
#	End

########  Waterz   #########################

	Event
		Ename	SubWater
		Type	Start
		At	MainFireBurst
		
		Part	:SubWaterSplashBurstStream.part
		Part	:SubWaterSplashBurst.part
		Part	:SubWaterSplash.part

		LifeSpan	70
	End

	Event
		Ename	SubWaterripples
		Type	start
		At	MainFireBurst
		

		Part	:SubWaterSplashBurstStream.part

		LifeSpan	50
	End
	
	Event
		Ename	SubWaterripples
		Type	start
		At	chest
		
		Bhvroverride
			PositionOffset	0 0 -5
		End

		Part	:SubWaterSplashBurstStream.part

		LifeSpan	50
	End
End

Condition
	On	Time
	Time	40

	Event
		Ename	SubWater
		Type	Start
		At	MainFireBurst
		
		Part	:LightSubWater.part
		Part	:LightSubWaterMist.part
		Part	:SubWaterSplashMist.part

		LifeSpan	30
	End

End

Condition
	On	Time
	Time	30

	Event
		Ename	SubWaterripples
		Type	start
		At	chest
		
		Bhvroverride
			PositionOffset	0 0 -5
		End

		Part	:SubWaterRipplesFront.part

		LifeSpan	50
	End

	Event
		Ename	SubWaterripples
		Type	start
		At	MainFireBurst

		Part	:SubWaterRipples.part

		LifeSpan	50
	End

End
###################################################################

Condition

	Event
		EName 	coreFire
		Type	local
		At	MainFireBurst
#		BhvrOverride
#			PositionOffset	0 5 0
#		End
		
		ChildFX	med_fire.fx

		Part1	:LargeCoreFireExplosion2.part
		Part2	:LargeCoreInferno.part
		Part2	:LargeCoreInferno2.part

		LifeSpan	15
	End

	Event
		EName 	coreFire
		Type	local
		At	MainFireBurst
#		BhvrOverride
#			PositionOffset	0 5 0
#		End
		
		Part1	:LargeCoreFireExplosion.part
		Part	:LargeCoreSmokeExplosion.part

		LifeSpan	55
	End

	Event
		EName 	base
		Type	local
		At	Chest
		BhvrOverride
			PositionOffset	0 8 -15
		End
		
	End

	Event
		EName 	baseTail
		Type	local
		At	Chest
		BhvrOverride
			PositionOffset	0 8 -75
		End
		
	End

	Event
		EName 	TailFin
		Type	local
		At	Chest
		BhvrOverride
			PositionOffset	0 8 -70
		End
		
	End

	Event
		EName 	Roter
		Type	local
		At	Chest
		BhvrOverride
			PositionOffset	0 12 -110
		End
		
	End

	Event
		EName 	TailFins
		Type	local
		At	Chest
		BhvrOverride
			PositionOffset	0 12 -95
		End
		
	End
	
	Event
		EName 	SubHatch
		Type	local
		At	Chest
		BhvrOverride
			PositionOffset	0 10.85 -55
		End
		
	End

	Event
		EName 	Mainfin
		Type	local
		At	Chest
		BhvrOverride
			PositionOffset	0 20 -15
		End
		
	End


	Event
		EName 	Sound
		Type	Posit
		At	base	
#		Sound	molotovhit1 80 30 .86
#		Sound	grexplo2 120.0 30.0 1.0

		LifeSpan	10
			
	End

	Event
		EName 	Fire
		Type	local
		At	base
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		ChildFX	med_fire.fx

		LifeSpan	25
	
	End

	Event
		EName 	Offset1
		Type	local
		At	base
		BhvrOverride
			PositionOffset	0 10 10
		End

#		LifeSpan	45
	
	End

	Event
		EName 	Offset3
		Type	local
		At	baseTail
		BhvrOverride
			PositionOffset	0 10 10
		End

#		LifeSpan	45
	
	End

	Event
		EName 	Flames1
		Type	Local
		At	Offset1
		ChildFX	med_fire.fx

		Part1	:FireExplosion2.part
		Part2	:Inferno.part
		Part	:ExplosionlightStreams.part
		Part1	:FireExplosion.part
		Part2	:Inferno2.part
		Part	:SmokeExplosion.part
		LifeSpan	15
	
	End

	Event
		EName 	Offset2
		Type	local
		At	base
		BhvrOverride
			PositionOffset	0 10 -10
		End

#		LifeSpan	45
	
	End

	Event
		EName 	Offset4
		Type	local
		At	baseTail
		BhvrOverride
			PositionOffset	0 10 -10
		End

#		LifeSpan	45
	
	End

	Event
		EName 	Flames2
		Type	local
		At	Offset2
		ChildFX	med_fire.fx

		Part1	:FireExplosion2.part
		Part2	:Inferno.part
		Part1	:FireExplosion.part
		Part2	:Inferno2.part
		Part	:SmokeExplosion.part
		LifeSpan	25
	
	End

	Event
		EName 	Smoke
		Type	local
		At	base
		
		Part	:SmokeExplosion.part
		
		Lifespan 90
	End

	Event
		EName 	Explosion
		Type	local
		At	base
		
		Part1	:FireExplosion2.part
		Part2	:Inferno.part
		Part	:ExplosionlightStreams.part
		Part1	:FireExplosion.part
		Part2	:Inferno2.part
		Part	:SmokeExplosion.part
		
		Lifespan 15
	End

	Event
		EName 	Explosion
		Type	local
		At	Tailfin
		
		Part1	:FireExplosion2.part
		Part2	:Inferno.part
		Part	:ExplosionlightStreams.part
		Part1	:FireExplosion.part
		Part2	:Inferno2.part
		Part	:SmokeExplosion.part
		
		Lifespan 15
	End

	Event
		EName 	Flash
		Type	local
		At	base
	
		Part	:FireExplosionFlash.part
		Part	:ExplosionStar.part

		Lifespan 10
		
	End

End

Condition
	On 	Time
	Time 	3

	Event
		EName 	chunks
		Type	Local 
		At	base
		Part1	Generic\InfernorockBits.part
		Part2	Generic\InfernorockBitz.part
		Lifespan	20
	End

End

#########################################################################

Condition

	Event
		EName 	Fire2
		Type	local
		At	TailFin
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		ChildFX	med_fire.fx

		LifeSpan	25
	
	End

	Event
		EName 	Offset3
		Type	local
		At	TailFin
		BhvrOverride
			PositionOffset	0 10 10
		End

#		LifeSpan	45
	
	End

	Event
		EName 	Flames3
		Type	Local
		At	Offset3
		ChildFX	med_fire.fx

		Part1	:FireExplosion2.part
		Part2	:Inferno.part
		Part	:ExplosionlightStreams.part
		Part1	:FireExplosion.part
		Part2	:Inferno2.part
		Part	:SmokeExplosion.part
		LifeSpan	25
	
	End

	Event
		EName 	Offset4
		Type	local
		At	TailFin
		BhvrOverride
			PositionOffset	0 10 -10
		End

#		LifeSpan	45
	
	End

	Event
		EName 	Flames4
		Type	local
		At	Offset4
		ChildFX	med_fire.fx

		Part1	:FireExplosion2.part
		Part2	:Inferno.part
		Part1	:FireExplosion.part
		Part2	:Inferno2.part
		Part	:SmokeExplosion.part
		LifeSpan	25
	
	End

	Event
		EName 	Smoke2
		Type	local
		At	TailFin
		
		Part	:SmokeExplosion.part
		
		Lifespan 90
	End

	Event
		EName 	Explosion2
		Type	local
		At	TailFin
		
		Part1	:FireExplosion2.part
		Part2	:Inferno.part
		Part	:ExplosionlightStreams.part
		Part1	:FireExplosion.part
		Part2	:Inferno2.part
		Part	:SmokeExplosion.part
		
		Lifespan 15
	End

	Event
		EName 	Flash2
		Type	local
		At	TailFin
	
		Part	:FireExplosionFlash.part
		Part	:ExplosionStar.part

		Lifespan 10
		
	End

End

Condition
	On 	Time
	Time 	3

	Event
		EName 	chunks2
		Type	Local 
		At	TailFin
		Part1	Generic\InfernorockBits.part
		Part2	Generic\InfernorockBitz.part
		Lifespan	20
	End

End

########   Generic Explosion Physics Particles   ###################
 
Condition
	On 	time
	Time 	0
	Repeat 	12

	Event 
		EName 	Sparkz
		Type 	Start
		At 	base
		ChildFX V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
		Lifespan 60
	End
End

Condition
	On 	time
	Time 	0
	Repeat 	30

	Event 
		EName 	Sparkzz
		Type 	Start
		At 	base
		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
		Lifespan 60
	End
End


Condition
	On 	time
	Time 	0
	Repeat  21

	Event 
		EName 	napalm
		Type 	Local
		At 	base
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On	Time
	Time 	3
	Repeat	6

	Event
		EName 	metal
		Type	Start
		At	base
		Geom	Metal_Curl
		Geom	Metal_Flat
		
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part

		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End

End

Condition
	On	Time
	Time 	3
	Repeat	15

	Event
		Ename	parts
		Type	Start
		At	base

		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		
		Part	V_COV\PhysicsEnabled\FireSplashBig.part
		
		Geom	carpart_sprocket
		Geom	carpart_pipe
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 0 2
			
			PyrRotateJitter		180 180 180

			Scale			1 1 1
			
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		0.0 0.8 0.0
			InitialVelocityJitter	1.2 0.5 1.2

			Drag			0.01

			physSFriction 	 	0.6
			
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	120
		LifespanJitter	30
	End

End

########################################################################

Condition
	On 	time
	Time 	0
	Repeat 	12

	Event 
		EName 	Sparkz
		Type 	Start
		At 	TailFin
		ChildFX V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
		Lifespan 60
	End
End

Condition
	On 	time
	Time 	0
	Repeat 	30

	Event 
		EName 	Sparkzz
		Type 	Start
		At 	TailFin
		ChildFX V_COV\PhysicsEnabled\HotSparkExplode.fx
		Lifespan 60
	End
End


Condition
	On 	time
	Time 	0
	Repeat  21

	Event 
		EName 	napalm
		Type 	Local
		At 	TailFin
		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On	Time
	Time 	3
	Repeat	6

	Event
		EName 	metal
		Type	Start
		At	TailFin
		Geom	Metal_Curl
		Geom	Metal_Flat
		
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part

		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			InitialVelocity		.0 0.3 .0
			InitialVelocityJitter	0.5 0.2 0.5
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	120
		LifespanJitter	30
	End

End

Condition
	On	Time
	Time 	3
	Repeat	15

	Event
		Ename	parts
		Type	Start
		At	TailFin

		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		
		Part	V_COV\PhysicsEnabled\FireSplashBig.part
		
		Geom	carpart_sprocket
		Geom	carpart_pipe
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 0 2
			
			PyrRotateJitter		180 180 180

			Scale			1 1 1
			
			Spin			0.0 0.0 0.0
			SpinJitter		0.5 0.5 0.5

			InitialVelocity		0.0 0.8 0.0
			InitialVelocityJitter	1.2 0.5 1.2

			Drag			0.01

			physSFriction 	 	0.6
			
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	120
		LifespanJitter	30
	End

End

###########################################################
########   Explosion Physics Objects   ###################
##########################################################

Condition
	Time 	0

#####################  Main Chassis  #########################

	Event
		Ename	Husk
		Type	Start
		At	base
		Part	:SmokeExplosion.part
		
		ChildFX	med_fire.fx
		
		Geom	Fx_NoseChunck
#		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
		
			Drag			.1
#			PositionOffset	0 0 30

			Scale			1 1 1
			Spin			0.0 0.0 0.0
			SpinJitter		0.02 0.02 0.02
			
			Gravity			.03

			InitialVelocity		0.0 0.35 0.0
			InitialVelocityJitter	0.25 0.2 0.25

#			physRestitution 	0.25

		End
#		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
#		CThresh 0.0001
		
		Lifespan	240
		LifespanJitter	30
	End

	Event
		EName 	EngineFire
		Type	Local
		At	Husk
		BhvrOverride
			PositionOffset	0 -7 21
		End
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		ChildFX	med_fire.fx
			
	End

	Event
		EName 	EngineFire
		Type	Local
		At	Husk
		BhvrOverride
			PositionOffset	0 10 -1.0
		End
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		ChildFX	med_fire.fx
			
	End

	Event
		EName 	EngineFire
		Type	Local
		At	Husk
		BhvrOverride
			PositionOffset	0 2 10
		End
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		ChildFX	med_fire.fx
			
	End

	Event
		EName 	LocalExplosion
		Type	Local
		At	Husk
		ChildFX	med_fire.fx

		Part1	:FireExplosion2.part
		Part2	:Inferno.part
		Part1	:FireExplosion.part
		Part2	:Inferno2.part
		Part	:SmokeExplosion.part
		LifeSpan	20
	
	End

#########################################

	Event
		Ename	TailHusk
		Type	Start
		At	baseTail
		Part	:SmokeExplosion.part
		
		ChildFX	med_fire.fx
		
		Geom	Fx_TailChunck
#		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
		
			Drag			.1
#			PositionOffset	0 0 -30

			Scale			1 1 1

			Spin			0.0 0.0 0.0
			SpinJitter		0.02 0.02 0.02
			
			Gravity			.03

			InitialVelocity		0.0 0.45 0.0
			InitialVelocityJitter	0.25 0.2 0.25

#			physRestitution 	0.25

		End
#		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
#		CThresh 0.0001
		
		Lifespan	240
		LifespanJitter	30
	End

	Event
		EName 	EngineFire
		Type	Local
		At	TailHusk
		BhvrOverride
			PositionOffset	0 -7 21
		End
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		ChildFX	med_fire.fx
			
	End

	Event
		EName 	EngineFire
		Type	Local
		At	TailHusk
		BhvrOverride
			PositionOffset	0 10 -1.0
		End
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		ChildFX	med_fire.fx
			
	End

	Event
		EName 	EngineFire
		Type	Local
		At	TailHusk
		BhvrOverride
			PositionOffset	0 2 10
		End
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		ChildFX	med_fire.fx
			
	End

	Event
		EName 	LocalExplosion
		Type	Local
		At	TailHusk
		ChildFX	med_fire.fx

		Part1	:FireExplosion2.part
		Part2	:Inferno.part
		Part1	:FireExplosion.part
		Part2	:Inferno2.part
		Part	:SmokeExplosion.part
		LifeSpan	20
	
	End

##################### Arms  ######################

	Event
		Ename	TopFin
		Type	Start
		At	TailFins
		
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		
		
		Geom	Fx_TailFin	#Fx_TopFin
#		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			StartJitter		1 1 1
			Scale			1 1 1
			
			PyrRotate		0 0 0
			PyrRotateJitter		360 360 360
			
			Gravity			.03

			Spin			0.0 0.0 0.0
			SpinJitter		0.15 0.15 0.15

			InitialVelocity		0 .5 0
			InitialVelocityJitter	0.4 0.4 0.4

#			physRestitution 	0.25
			
		End
#		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
#		CThresh 0.0001
		
		Lifespan	210
		LifespanJitter	60
	End

	Event
		EName 	LocalExplosion
		Type	Local
		At	TopFin
		ChildFX	med_fire.fx

		Part1	:FireExplosion2.part
		Part2	:Inferno.part
		Part1	:FireExplosion.part
		Part2	:Inferno2.part
		Part	:SmokeExplosion.part
		LifeSpan	8
	
	End

###############################################################################

	Event
		Ename	Rudder
		Type	Start
		At	Roter
		
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		
		
		Geom	Fx_TailRoater
#		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			StartJitter		1 1 1
#			PositionOffset		-12 -1 1
			
			Scale			1 1 1
			
			Gravity			.03

			PyrRotate		0 0 0
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.15 0.15 0.15

			InitialVelocity		0 .5 0
			InitialVelocityJitter	0.4 0.4 0.4
			
#			physRestitution 	0.25
		End
#		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
#		CThresh 0.0001
		
		Lifespan	210
		LifespanJitter	60
	End

	Event
		EName 	LocalExplosion
		Type	Local
		At	Rudder
		ChildFX	med_fire.fx

		Part1	:FireExplosion2.part
		Part2	:Inferno.part
		Part1	:FireExplosion.part
		Part2	:Inferno2.part
		Part	:SmokeExplosion.part
		LifeSpan	20
	
	End

########################################################################

	Event
		Ename	Rudder
		Type	Start
		At	subhatch
		
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		
		
		Geom	SewrHatch_Base
#		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
#			StartJitter		1 1 1
#			PositionOffset		-12 -1 1
			
			Scale			1 1 1
			
			Gravity			.03

			PyrRotate		0 0 0
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.15 0.15 0.15

			InitialVelocity		0 .5 0
			InitialVelocityJitter	0.4 0.4 0.4
			
#			physRestitution 	0.25
		End
#		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
#		CThresh 0.0001
		
		Lifespan	210
		LifespanJitter	60
	End

	Event
		EName 	LocalExplosion
		Type	Local
		At	Rudder
		ChildFX	med_fire.fx

		Part1	:FireExplosion2.part
		Part2	:Inferno.part
		Part1	:FireExplosion.part
		Part2	:Inferno2.part
		Part	:SmokeExplosion.part
		LifeSpan	20
	
	End

	Event
		Ename	Rudder
		Type	Start
		At	subhatch
		
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		
		
		Geom	SewerAccess_Hatch
#		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
#			StartJitter		1 1 1
#			PositionOffset		-12 -1 1
			
			Scale			1 1 1
			
			Gravity			.03

			PyrRotate		0 0 0
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.15 0.15 0.15

			InitialVelocity		0 .5 0
			InitialVelocityJitter	0.4 0.4 0.4
			
#			physRestitution 	0.25
		End
#		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
#		CThresh 0.0001
		
		Lifespan	210
		LifespanJitter	60
	End

	Event
		EName 	LocalExplosion
		Type	Local
		At	Rudder
		ChildFX	med_fire.fx

		Part1	:FireExplosion2.part
		Part2	:Inferno.part
		Part1	:FireExplosion.part
		Part2	:Inferno2.part
		Part	:SmokeExplosion.part
		LifeSpan	20
	
	End

#####################  Fins  ###############################

	Event
		Ename	TopFin
		Type	Start
		At	TailFins
		
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		
		
		Geom	Fx_TailFin01	#Fx_TopFin
#		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			StartJitter		1 1 1
#			PositionOffset		0 3 0
			
			Scale			1 1 1
			
			Gravity			.03
			
			PyrRotate		0 0 0
			PyrRotateJitter		360 360 360

			Spin			0.0 0.0 0.0
			SpinJitter		0.15 0.15 0.15

			InitialVelocity		0 .5 0
			InitialVelocityJitter	0.4 0.4 0.4

#			physRestitution 	0.25
			
		End
#		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
#		CThresh 0.0001
		
		Lifespan	210
		LifespanJitter	60
	End

	Event
		EName 	LocalExplosion
		Type	Local
		At	TopFin
		ChildFX	med_fire.fx

		Part1	:FireExplosion2.part
		Part2	:Inferno.part
		Part1	:FireExplosion.part
		Part2	:Inferno2.part
		Part	:SmokeExplosion.part
		LifeSpan	8
	
	End


	Event
		Ename	TopFin2
		Type	Start
		At	Mainfin
		
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		
		
		Geom	Fx_TopFin
#		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			StartJitter		1 1 1
#			PositionOffset		0 3 0
			
			Gravity			.03
			Scale			1 1 1
			
			PyrRotate		0 0 0
			PyrRotateJitter		360 360 360

			InitialVelocity		0 .5 0
			SpinJitter		0.1 0.1 0.1

			InitialVelocity		0 .5 0
			InitialVelocityJitter	0.4 0.4 0.4

#			physRestitution 	0.25
			
		End
#		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
#		CThresh 0.0001
		
		Lifespan	210
		LifespanJitter	60
	End

	Event
		EName 	LocalExplosion
		Type	Local
		At	TopFin2
		ChildFX	med_fire.fx

		Part1	:FireExplosion2.part
		Part2	:Inferno.part
		Part1	:FireExplosion.part
		Part2	:Inferno2.part
		Part	:SmokeExplosion.part
		LifeSpan	8
	
	End

End

End			