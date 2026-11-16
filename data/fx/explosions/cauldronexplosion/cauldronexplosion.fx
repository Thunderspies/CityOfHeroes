#########################################################
##	onfire
##
FxInfo  

LifeSpan	800



Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	chest
		
		Sound CauldronExplo 90 90 .98

	End


	Event
		Ename	Base
		Type	Start
		At	Root

		BhvrOverride
			PositionOffset	0 3 0
		End
	
	End

	
	Event
		Type	start 
		At	Base
	
		BhvrOverride
		End
		
		Part	Explosions\VehicleExplosion\FireExplosionFlash.part
		#Part	Explosions\VehicleExplosion\ExplosionlightStreams.part

		Part	Explosions\VehicleExplosion\SmokeExplosion.part
		
		Part1	Explosions\VehicleExplosion\FireBallTargetSteam.part
		Part3	Explosions\VehicleExplosion\FireBallSpark.part

		LifeSpan	3

	End
	
	Event
		Type	start 
		At	Base
	
		BhvrOverride
		End

		Part	Explosions\VehicleExplosion\SmokeExplosion.part
		
		Part1	Explosions\VehicleExplosion\FireBallTargetSteam.part
		Part3	Explosions\VehicleExplosion\FireBallSpark.part

		LifeSpan	30

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
		Lifespan	10
	End

End

Condition
	On 	Time
	Time 	0
	Repeat	7

	Event
		Type	start 
		At	Base
	
		BhvrOverride
			StartJitter	5 4 5
		End
		
		Part	Explosions\MetalExplosion\PHS_MetalExplosionBig.part
		Part	Explosions\MetalExplosion\PHS_MetalExplosion01.part

		
		Part1	Explosions\VehicleExplosion\FireBallTargetSteam.part
		Part3	Explosions\VehicleExplosion\FireBallSpark.part

		Lifespan	15

	End

End

########   Generic Explosion Physics Particles   ###################
 
Condition
	On 	time
	Time 	0
	Repeat 	7

	Event 
		EName 	Sparkz
		Type 	Start
		At 	base
		
		BhvrOverride
			PositionOffset	0 2 0
		End

		ChildFX V_COV\PhysicsEnabled\HotSparkOUT.fx
		Lifespan 60
	End
End

Condition
	On 	time
	Time 	0
	Repeat 	15

	Event 
		EName 	Sparkzz
		Type 	Start
		At 	base
		
		BhvrOverride
			PositionOffset	0 2 0
		End

		ChildFX V_COV\PhysicsEnabled\MetalSparkOut.fx
		Lifespan 60
	End
End


############   Cauldron   ################################

Condition
	On 	Time
	Time 	0

	Repeat	9

	Event
		Ename	Grate
		Type	start
		At	Base
		Geom	exp_cauldron_grill

		
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 3 0		
			StartJitter		4 0 4	
			scale			1 1 1

			InitialVelocity		0 0.65 0
			InitialVelocityJitter	0.5 0.25 0.5

			PyrRotateJitter		0.2 0.2 0.2
			SpinJitter		.2 .4 .2

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.8
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			FadeOutLength		510
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1120
		LifespanJitter	30
	End

End


Condition
	On 	Time
	Time 	0

	Event
		Ename	panelL
		Type	start
		At	Base
	
		Geom	exp_cauldron_chunk01
		
		Bhvr	:PhysicsDebrisFadeStart270.bhvr
		BhvrOverride
			PositionOffset		4 0 0		
			scale			1 1 1

			InitialVelocity		.4 0.25 0
			InitialVelocityJitter	0.25 0.15 0.25
			
			Spin			0 0 -.15
			SpinJitter		.2 .1 .1

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.8
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			FadeOutLength		510
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1120
		LifespanJitter	30
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Ename	panelR
		Type	start
		At	Base
	
		Geom	exp_cauldron_chunk01
		
		Bhvr	:PhysicsDebrisFadeStart90.bhvr
		BhvrOverride
			PositionOffset		-4 0 0
			scale			1 1 1

			InitialVelocity		-.4 0.25 0
			InitialVelocityJitter	0.25 0.15 0.25
			
			Spin			0 0 .15
			SpinJitter		.2 .1 .1

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.8
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			FadeOutLength		510
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1120
		LifespanJitter	30
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Ename	panelF
		Type	start
		At	Base
	
		Geom	exp_cauldron_chunk01
		
		Bhvr	:PhysicsDebrisFadeStart180.bhvr
		BhvrOverride
			PositionOffset		0 0 4
			scale			1 1 1

			InitialVelocity		0 0.25 .4
			InitialVelocityJitter	0.25 0.15 0.25

			Spin			.15 0 0
			SpinJitter		.1 .1 .2

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.8
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			FadeOutLength		510
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1120
		LifespanJitter	30
	End

End

Condition
	On 	Time
	Time 	0

	Event
		Ename	panelB
		Type	start
		At	Base
	
		Geom	exp_cauldron_chunk01
		
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 0 -4	
			scale			1 1 1

			InitialVelocity		0 0.25 -.4
			InitialVelocityJitter	0.25 0.15 0.25
			
			Spin			-.15 0 0
			SpinJitter		.1 .1 .2

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.8
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			FadeOutLength		510
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1120
		LifespanJitter	30
	End

End

Condition
	On 	Time
	Time 	0

	Repeat	7

	Event
		Ename	chunks
		Type	start
		At	Base

		Geom	exp_cauldron_chunk02
		Geom	exp_cauldron_chunk03
		Geom	exp_cauldron_chunk04
		
		Bhvr	:PhysicsDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 0 0
			StartJitter		4 1.5 4		
			scale			.8 .8 .8

			InitialVelocity		0 0.5 0
			InitialVelocityJitter	0.75 0.25 0.75

			PyrRotateJitter		0 180 0
			SpinJitter		.2 .2 .2

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.8
			physRestitution 	0.1
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			FadeOutLength		510
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1120
		LifespanJitter	30
	End

End

######################  SOUP   #########################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	liquid
		Type	start
		At	Base
		Geom	exp_cauldron_liquid

		
		Bhvr	:CauldronLiquid.bhvr
		BhvrOverride
			PositionOffset		0 -2 0

			SpinJitter		0 .01 0
		End

		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001

		Lifespan	1
	End

End

#Condition
#	On 	Time
#	Time 	30
#
#	Event
#		Ename	stain
#		Type	start
#		At	Base
#		Geom	exp_cauldron_liquid
#
#		
#		Bhvr	:soupStain.bhvr
#		BhvrOverride
#			PositionOffset		0 -2.8 0
#
#			Scale			3.3 .1 3.3
#
#			gravity			0
#
#			Alpha			50
#
#		End
#
#		Lifespan	15
#	End
#
#End
#
Condition
	On 	Time
	Time 	0

	Repeat	11

	Event
		Ename	drops
		Type	start
		At	Base

		Geom	exp_cauldron_glob
		
		Bhvr	:soupFade.bhvr
		BhvrOverride
			StartJitter		4 1.5 4		
			scale			.5 .5 .5

			InitialVelocity		0 0.5 0
			InitialVelocityJitter	0.75 0.5 0.75

			SpinJitter		.2 .2 .2
		End

		CEvent 		:SoupSplat.fx
		CThresh		0.0001
		Cdestroy	1

		Lifespan	1
	End

End

Condition
	On 	Time
	Time 	10

	Repeat	15

	Event
		Ename	drops
		Type	start
		At	Base

		Geom	exp_cauldron_glob
		
		Bhvr	:soupFade.bhvr
		BhvrOverride
			PositionOffset		0 -2.8 0
			StartJitter		8 0 8	

			InitialVelocity		0 0.75 0
			InitialVelocityJitter	0.25 0.5 0.25

			SpinJitter		.2 .2 .2
		End

		CEvent 		:SoupSplatSmall.fx
		CThresh		0.0001
		Cdestroy	1

		Lifespan	1
	End

End

End			