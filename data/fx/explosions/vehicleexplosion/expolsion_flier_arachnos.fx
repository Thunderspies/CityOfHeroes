#########################################################
##	car on fire
#########################################################

FxInfo  

Lifespan 300

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	chest
		
		Sound flierexplo1 200 200 .93

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
			PhysForcePower		500
			PhysForcePowerJitter	50
		End

		Lifespan	1

	End

End

########  Main Explosion Particles   #########################

Condition
	On 	Time
	Time 	0


	Event
		EName 	base
		Type	Create
		Inherit	Position Rotation
		Update	Position
		At	Chest
		BhvrOverride
			PositionOffset	0 5 0
		End
		
	End



	Event
		EName 	Sound
		Type	Posit
		At	base	
		Sound molotovhit1 80 80 .86
		#Sound	fireexplo 80 30 .72
		#Sound	firezap 80 30 .76
		#Sound	molotovhit1 80 30 .86
		Sound grexplo2 120.0 120.0 1.0

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

		LifeSpan	45
	
	End

	Event
		EName 	Offset1
		Type	local
		At	base
		BhvrOverride
			PositionOffset	0 10 10
		End

		LifeSpan	45
	
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
		LifeSpan	45
	
	End

	Event
		EName 	Offset2
		Type	local
		At	base
		BhvrOverride
			PositionOffset	0 10 -10
		End

		LifeSpan	45
	
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
		LifeSpan	45
	
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
		
		Lifespan 35
	End


#	Event
#		EName 	flat
#		Type	Local 
#		At	Root
#		
#		Part2	:FlatInferno.part
#		Part	:FlatExplosionStar.part
#		LifeSpan	10
#
#	End
#
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
		
		Geom	exp_arachflier_main
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.02 0.0 0.0
			SpinJitter		0.03 0.03 0.02

			InitialVelocity		0.0 0.35 0.6
			InitialVelocityJitter	0.15 0.2 0.35

			physRestitution 	0.25

		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
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
		Type	Start
		At	Husk
		ChildFX	med_fire.fx

		Part1	:FireExplosion2.part
		Part2	:Inferno.part
		Part1	:FireExplosion.part
		Part2	:Inferno2.part
		Part	:SmokeExplosion.part
		LifeSpan	30
	
	End

##################### Arms  ######################

	Event
		Ename	NacelleRight
		Type	Start
		At	base
		
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		
		
		Geom	exp_arachflier_arm_R
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		12 -1 1
			
			Scale			1 1 1
			
			PyrRotate		0 0 0
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.15 0.05 0.05

			InitialVelocity		.5 .15 -.1
			InitialVelocityJitter	0.2 0.3 0.4

			physRestitution 	0.25
			
		End
		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 0.0001
		
		Lifespan	210
		LifespanJitter	60
	End

	Event
		EName 	LocalExplosion
		Type	Start
		At	NacelleRight
		ChildFX	med_fire.fx

		Part1	:FireExplosion2.part
		Part2	:Inferno.part
		Part1	:FireExplosion.part
		Part2	:Inferno2.part
		Part	:SmokeExplosion.part
		LifeSpan	30
	
	End

	Event
		Ename	NacelleLeft
		Type	Start
		At	base
		
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		
		
		Geom	exp_arachflier_arm_L
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-12 -1 1
			
			Scale			1 1 1

			PyrRotate		0 0 0
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.15 0.05 0.05

			InitialVelocity		-.5 .15 -.1
			InitialVelocityJitter	0.2 0.3 0.4
			
			physRestitution 	0.25
		End
		CEvent 	V_COV\PhysicsEnabled\FireNapalmHit.fx
		CThresh 0.0001
		
		Lifespan	210
		LifespanJitter	60
	End

	Event
		EName 	LocalExplosion
		Type	Start
		At	NacelleLeft
		ChildFX	med_fire.fx

		Part1	:FireExplosion2.part
		Part2	:Inferno.part
		Part1	:FireExplosion.part
		Part2	:Inferno2.part
		Part	:SmokeExplosion.part
		LifeSpan	30
	
	End

####################  Hump  ###############################

	Event
		Ename	Hump
		Type	Start
		At	base
		Part	:SmokeExplosion.part
		Part3	:FireBallSpark.part
		Part1	:FireBallTargetSteam.part
		
		Geom	exp_arachflier_hump
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 14 -10
		
			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.15 0.1 0.05

			InitialVelocity		0.0 1.2 0.0
			InitialVelocityJitter	0.1 0.3 0.25

			physRestitution 	0.25
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	210
		LifespanJitter	60
	End

	Event
		EName 	LocalExplosion
		Type	Start
		At	Hump
		ChildFX	med_fire.fx

		Part1	:FireExplosion2.part
		Part2	:Inferno.part
		Part	:ExplosionlightStreams.part
		Part1	:FireExplosion.part
		Part2	:Inferno2.part
		Part	:SmokeExplosion.part
		LifeSpan	30
	
	End

####################  Wings  ###############################

	Event
		Ename	WingRight
		Type	Start
		At	base

		Part3	:FireBallSpark.part
		Part1	:FireBallTargetSteam.part
		Part	:SmokeExplosion.part
		Part2	:FireBallTargetFlame2.part
		ChildFX	med_fire.fx
		
		Geom	exp_arachflier_wing_R
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		20 12 -5
	
			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.05 0.15 0.05

			InitialVelocity		0.7 0.6 0.2
			InitialVelocityJitter	0.3 0.4 0.3

			physRestitution 	0.25
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	210
		LifespanJitter	60
	End

	Event
		Ename	WingLeft
		Type	Start
		At	base

		Part3	:FireBallSpark.part
		Part1	:FireBallTargetSteam.part
		Part	:SmokeExplosion.part
		Part2	:FireBallTargetFlame2.part
		ChildFX	med_fire.fx
		
		Geom	exp_arachflier_wing_L
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-20 12 -1
	
			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.05 0.15 0.05

			InitialVelocity		-0.7 0.6 0.2
			InitialVelocityJitter	0.3 0.4 0.3

			physRestitution 	0.25
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	210
		LifespanJitter	60
	End


##################### WingRotors  ######################
#
#	Event
#		Ename	WingRotorRight
#		Type	Start
#		At	base
#		
#		Part1	:FireBallTargetSteam.part
#		#Part2	:FireBallTargetFlame2.part
#		Part3	:FireBallSpark.part
#		
#		
#		Geom	exp_arachflier_prop
#		Bhvr	:SkiffPhysicsDebrisFade.bhvr
#		BhvrOverride
#			PositionOffset		1 1 0
#		
#			Scale			1 1 1
#			PyrRotateJitter		11 11 11
#
#			Spin			0.0 -9.9 0.0
#			#SpinJitter		0.15 0.25 0.15
#
#			InitialVelocity		0.2 0.9 -0.3
#			InitialVelocityJitter	0.2 0.3 0.3
#
#			Gravity			.03
#			PhysGravity		.7
#
#		End
#		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
#		CThresh 0.0001
#		
#		Lifespan	180
#		LifespanJitter	30
#	End
#
#	Event
#		Ename	WingRotorLeft
#		Type	Start
#		At	base
#		
#		Part1	:FireBallTargetSteam.part
#		#Part2	:FireBallTargetFlame2.part
#		Part3	:FireBallSpark.part
#		
#		
#		Geom	exp_arachflier_prop
#		Bhvr	:SkiffPhysicsDebrisFade.bhvr
#		BhvrOverride
#			PositionOffset		-1 1 0
#		
#			Scale			1 1 1
#			
#			PyrRotate		0 0 180
#			PyrRotateJitter		11 11 11
#
#			Spin			0.0 9.9 0.0
#			#SpinJitter		0.15 0.25 0.15
#
#			InitialVelocity		-0.2 0.9 -0.3
#			InitialVelocityJitter	0.2 0.3 0.3
#
#			Gravity			.03
#			PhysGravity		.7
#
#		End
#		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
#		CThresh 0.0001
#		
#		Lifespan	180
#		LifespanJitter	30
#	End
#
####################  Tail  ###############################

	Event
		Ename	Tail
		Type	Start
		At	base
		Part	:SmokeExplosion.part
		Part3	:FireBallSpark.part
		Part1	:FireBallTargetSteam.part
		
		Geom	exp_arachflier_tail
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 10 -30
		
			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			-0.05 0.0 0.0
			SpinJitter		0.1 0.1 0.05

			InitialVelocity		0.0 0.6 -0.8
			InitialVelocityJitter	0.1 0.3 0.32

			physRestitution 	0.25
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	210
		LifespanJitter	60
	End

	Event
		EName 	LocalExplosion
		Type	Start
		At	Tail
		ChildFX	med_fire.fx

		Part1	:FireExplosion2.part
		Part2	:Inferno.part
		Part	:ExplosionlightStreams.part
		Part1	:FireExplosion.part
		Part2	:Inferno2.part
		Part	:SmokeExplosion.part
		LifeSpan	30
	
	End

##################### TailRotors  ######################
#
#	Event
#		Ename	TailRotorRight
#		Type	Start
#		At	base
#		
#		Part1	:FireBallTargetSteam.part
#		#Part2	:FireBallTargetFlame2.part
#		Part3	:FireBallSpark.part
#		
#		
#		Geom	exp_arachflier_prop
#		Bhvr	:SkiffPhysicsDebrisFade.bhvr
#		BhvrOverride
#			PositionOffset		1 1 -6
#		
#			Scale			1 1 1
#			PyrRotateJitter		11 11 11
#
#			Spin			0.0 -9.9 0.0
#			#SpinJitter		0.15 0.25 0.15
#
#			InitialVelocity		0.2 0.9 -0.3
#			InitialVelocityJitter	0.2 0.3 0.3
#
#			Gravity			.03
#			PhysGravity		.7
#
#		End
#		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
#		CThresh 0.0001
#		
#		Lifespan	180
#		LifespanJitter	30
#	End
#
#	Event
#		Ename	TailRotorLeft
#		Type	Start
#		At	base
#		
#		Part1	:FireBallTargetSteam.part
#		#Part2	:FireBallTargetFlame2.part
#		Part3	:FireBallSpark.part
#		
#		
#		Geom	exp_arachflier_prop
#		Bhvr	:SkiffPhysicsDebrisFade.bhvr
#		BhvrOverride
#			PositionOffset		-1 1 -6
#		
#			Scale			1 1 1
#			
#			PyrRotate		0 0 180
#			PyrRotateJitter		11 11 11
#
#			Spin			0.0 9.9 0.0
#			#SpinJitter		0.15 0.25 0.15
#
#			InitialVelocity		-0.2 0.9 -0.3
#			InitialVelocityJitter	0.2 0.3 0.3
#
#			Gravity			.03
#			PhysGravity		.7
#
#		End
#		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
#		CThresh 0.0001
#		
#		Lifespan	180
#		LifespanJitter	30
#	End
#
#
####################  GunPod  ###############################

	Event
		Ename	GunPod
		Type	Start
		At	base

		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		Part	:SmokeExplosion.part
		
		Geom	exp_arachflier_gunpod
		Bhvr	:SkiffPhysicsDebris.bhvr
		BhvrOverride
			PositionOffset		0 -12 20
		
			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.15 0.15 0.15

			InitialVelocity		0.0 -0.2 0.4
			InitialVelocityJitter	0.3 0.4 0.2

			physRestitution 	0.25
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	150
		LifespanJitter	30
	End

	Event
		Ename	GunsLeft
		Type	Start
		At	GunPod
		
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		
		Geom	exp_arachflier_guns
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		-1.5 1 -1
	
			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35

			InitialVelocity		-0.5 -0.4 0.4
			InitialVelocityJitter	0.3 0.4 0.2

			physRestitution 	0.25
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	120
		LifespanJitter	30
	End

	Event
		Ename	GunsRight
		Type	Start
		At	GunPod
		
		Part1	:FireBallTargetSteam.part
		Part2	:FireBallTargetFlame2.part
		Part3	:FireBallSpark.part
		
		Geom	exp_arachflier_guns
		Bhvr	:SkiffPhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		1.5 1 -1
	
			Scale			1 1 1
			PyrRotateJitter		0 0 0

			Spin			0.0 0.0 0.0
			SpinJitter		0.35 0.35 0.35

			InitialVelocity		0.5 -0.4 0.4
			InitialVelocityJitter	0.3 0.4 0.2

			physRestitution 	0.25
		End
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		
		Lifespan	120
		LifespanJitter	30
	End


End


#######################################################################
#######################################################################

End			