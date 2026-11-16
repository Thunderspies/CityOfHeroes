#########################################################
## Rikti Shuttle - Explosion/Death FX
#########################################################

FxInfo  

Lifespan 300

## SOUND FX #######################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start 
		At	Chest
		Sound flierexplo1 400 400 .93
	End

End

## EXPLOSION #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Flash
		Type	Local
		At	Origin
		Part	:Explosion_Secondary_Flash1.part
		Part	:Explosion_Secondary_Flash2.part
		Lifespan	10
	End

	Event
		EName 	Flash
		Type	Local
		At	Origin
		BhvrOverride
			PyrRotateJitter	22.5 0 22.5
		End
		Part	:Explosion_Secondary_Shockwave1.part
		Lifespan	10
	End

	Event
		EName 	Flames1
		Type	Local
		At	Origin
		Part	:Explosion_Secondary_Smoke.part
		Part2	:Explosion_Secondary_Plasma.part
		Part2	:Explosion_Secondary_Plasma2.part
		#Part1	:FireExplosion2.part
		#Part2	:Inferno.part
		LifeSpan	4
	End

	Event
		EName 	Flames1
		Type	Local
		At	Origin
		Part	:Explosion_Secondary_Streaks1.part
		Part	:Explosion_Secondary_Streaks2.part
		LifeSpan	3
	End
End

## SPARKS #######################################################
 
#Condition
#	On 	time
#	Time 	0
#	Repeat 	8
#
#	Event 
#		EName 	Sparks
#		Type 	Start
#		At 	Origin
#		ChildFX V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
#		Lifespan	60
#		#LifespanJitter 	30
#	End
#End
#
#Condition
#	On 	time
#	Time 	0
#	Repeat  4
#
#	Event 
#		EName 	FierySparks
#		Type 	Start
#		At 	Origin
#		ChildFX V_COV\PhysicsEnabled\FireBurstExplode.fx
#		Lifespan	90
#		LifespanJitter 	30
#	End
#End

## FLAMING TENDRILS #######################################################

#Condition
#	On	Time
#	Time 	3
#	Repeat	2
#
#	Event
#		EName 	metal
#		Type	Start
#		At	Origin
#		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
#		BhvrOverride
#			PyrRotateJitter		180 180 180
#			InitialVelocity		0.0 0.0 0.0
#			InitialVelocityJitter	0.5 0.375 0.5
#			Spin			0.0 0.0 0.0
#			SpinJitter		0.5 0.5 0.5
#			Drag			0.01
#		End
#		Geom	Metal_Curl
#		Geom	Metal_Flat
#		Part3	Explosions\VehicleExplosion\FireBallSpark.part
#		Part	:SmokeTrail2.part
#		Lifespan	120
#		LifespanJitter	30
#	End
#End
#
#Condition
#	On	Time
#	Time 	3
#	Repeat	2
#
#	Event
#		Ename	parts
#		Type	Start
#		At	Origin
#		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
#		BhvrOverride
#			PyrRotateJitter		180 180 180
#			InitialVelocity		0.0 0.5 0.0
#			InitialVelocityJitter	1.0 0.5 1.5
#			Scale			1 1 1
#			Spin			0.0 0.0 0.0
#			SpinJitter		0.5 0.5 0.5
#			Drag			0.01
#		End
#		Geom	carpart_sprocket
#		Geom	carpart_pipe
#		Part3	Explosions\VehicleExplosion\FireBallSpark.part
#		Part	:SmokeTrail2.part
#		Part	V_COV\PhysicsEnabled\FireSplashBig.part
#		Lifespan	120
#		LifespanJitter	30
#	End
#End

#######################################################################

End