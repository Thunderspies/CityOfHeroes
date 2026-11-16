#########################################################
##	PhysicsRing
##
FxInfo  

#LifeSpan 100

Condition

	On	Time
	Time	0

	Event
		Type	Local
		At	origin
		Sound portalexplo 220 220 .8
	End
End


Condition

	On	Time
	Time	0

	Event
		Type	Local
		At	origin
		Sound dumpsterexplo3 220 220 .6
	End
End

###############################################################################
##Explosion
##################################

Condition
	Time 	0

		Event
		Type	local
		At	root
		
		BHVR	Behaviors/VaultCameraShake.bhvr
				
		LifeSpan	300
	End

	Event
		EName	VaultHookup
		Type	local
		At	origin
		
		Bhvroverride
			PositionOffset	0 1 -13
		End

		BHVR	Behaviors/GenericParticleFade.bhvr
		Geom	VaultHookup
		
		LifeSpan	300
	End

	Event
		EName 	Prime1
		Type	Local 
		At	VaultHookup
		Altpiv	4
		
		Part	:InfernoCare.part
		Part	:InfernoCare.part
		Part	:Inferno.part
		Part	:Sparks.part
		
		LifeSpan	200

	End

	Event
		EName 	Prime2
		Type	Local 
		At	VaultHookup
		Altpiv	4
		Part	:InfernoExplosion.part
		Part	:SparksSmall.part
		
		LifeSpan	280

	End

End
#########################################
################################3

Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		BhvrOverride
			StartJitter		6 6 6
			InitialVelocity		0.0 1.2 0.0			
			InitialVelocityJitter	0.8 0.25 0.8
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1

		End

		Bhvr	:Phys1.bhvr
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	260
		LifespanJitter	100
	End

End

Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		BhvrOverride

			StartJitter		6 6 6
			InitialVelocity		0.0 1.2 0.0			
			InitialVelocityJitter	0.8 0.25 0.8
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1

		End

		Bhvr	:Phys1.bhvr
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	260
		LifespanJitter	100
	End

End

Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		BhvrOverride

			StartJitter		6 6 6
			InitialVelocity		0.0 1.2 0.0			
			InitialVelocityJitter	0.8 0.25 0.8
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1

		End

		Bhvr	:Phys1.bhvr
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	260
		LifespanJitter	100
	End

End

Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		BhvrOverride

			StartJitter		6 6 6
			InitialVelocity		0.0 1.2 0.0			
			InitialVelocityJitter	0.8 0.25 0.8
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1

		End

		Bhvr	:Phys1.bhvr
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	260
		LifespanJitter	100
	End

End

Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		BhvrOverride

			StartJitter		6 6 6
			InitialVelocity		0.0 1.2 0.0			
			InitialVelocityJitter	0.8 0.25 0.8
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1

		End

		Bhvr	:Phys1.bhvr
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	260
		LifespanJitter	100
	End

End

Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		BhvrOverride

			StartJitter		6 6 6
			InitialVelocity		0.0 1.2 0.0			
			InitialVelocityJitter	0.8 0.25 0.8
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1

		End

		Bhvr	:Phys1.bhvr
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	260
		LifespanJitter	100
	End

End

Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		BhvrOverride

			StartJitter		6 6 6
			InitialVelocity		0.0 1.2 0.0			
			InitialVelocityJitter	0.8 0.25 0.8
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1

		End

		Bhvr	:Phys1.bhvr
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	260
		LifespanJitter	100
	End

End

Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		BhvrOverride

			StartJitter		6 6 6
			InitialVelocity		0.0 1.2 0.0			
			InitialVelocityJitter	0.8 0.25 0.8
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1

		End

		Bhvr	:Phys1.bhvr
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	260
		LifespanJitter	100
	End

End

Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		BhvrOverride

			StartJitter		6 6 6
			InitialVelocity		0.0 1.2 0.0			
			InitialVelocityJitter	0.8 0.25 0.8
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1

		End

		Bhvr	:Phys1.bhvr
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	260
		LifespanJitter	100
	End

End

Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		BhvrOverride

			StartJitter		6 6 6
			InitialVelocity		0.0 1.2 0.0			
			InitialVelocityJitter	0.8 0.25 0.8
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1

		End

		Bhvr	:Phys1.bhvr
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	260
		LifespanJitter	100
	End

End

Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		BhvrOverride

			StartJitter		6 6 6
			InitialVelocity		0.0 1.2 0.0			
			InitialVelocityJitter	0.8 0.25 0.8
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1

		End

		Bhvr	:Phys1.bhvr
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	260
		LifespanJitter	100
	End

End

Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		BhvrOverride

			StartJitter		6 6 6
			InitialVelocity		0.0 1.2 0.0			
			InitialVelocityJitter	0.8 0.25 0.8
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1

		End

		Bhvr	:Phys1.bhvr
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	260
		LifespanJitter	100
	End

End

Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		BhvrOverride

			StartJitter		6 6 6
			InitialVelocity		0.0 1.2 0.0			
			InitialVelocityJitter	0.8 0.25 0.8
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1

		End

		Bhvr	:Phys1.bhvr
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	260
		LifespanJitter	100
	End

End

Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		BhvrOverride

			StartJitter		6 6 6
			InitialVelocity		0.0 1.2 0.0			
			InitialVelocityJitter	0.8 0.25 0.8
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1

		End

		Bhvr	:Phys1.bhvr
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	260
		LifespanJitter	100
	End

End

Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		BhvrOverride

			StartJitter		6 6 6
			InitialVelocity		0.0 1.2 0.0			
			InitialVelocityJitter	0.8 0.25 0.8
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1

		End

		Bhvr	:Phys1.bhvr
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	260
		LifespanJitter	100
	End

End

Condition
	Time 	0
	Chance	1

	Event
		Type	start
		At	VaultHookup
		Altpiv	4

		BhvrOverride

			StartJitter		6 6 6
			InitialVelocity		0.0 1.2 0.0			
			InitialVelocityJitter	0.8 0.25 0.8
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1

		End

		Bhvr	:Phys1.bhvr
		Part1	:fire.part
		Part1	:fire.part
		Lifespan	260
		LifespanJitter	100
	End

End

######################################################################################################

Condition
	On 	Time
	Time	15

	Event
		EName 	Prime1
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	3

	Event
		EName 	Prime2
		Type	Destroy
	End

End


Condition
	On 	Time
	Time	7

	Event
		EName 	Prime
		Type	Destroy
	End

End	

Condition
	On 	Time
	Time 	30

	Event
		EName 	RadiusFxScale
		Type	Destroy 
	End


End

################################################################
###Metal Teeth 
###############################################

Condition
	On 	Time
	Time 	0

	Event
		Ename	ToothHookup
		Type	local
		At	Origin
		
		BhvrOverride
			PositionOffset	0 13 -15
		End

	End

	Event
		Type	start 
		At	ToothHookup
		
		Bhvr	Behaviors/FadeOut.bhvr

		Part1	Powers/Claws/Clawflash.part
		Part3	:Embers.part
		Part4	:EmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part

		Part1	:fire.part
		Part1	:fire.part

		BhvrOverride
#			PositionOffset	1 12.5 -2
#			startJitter	1.3 1.3 1.3
			Scale		.6 .8 .6
			
			startcolor		85 75 55

			InitialVelocity		0.0 1.2 0.0			
			InitialVelocityJitter	0.8 0.25 0.8
			Spin			.1 .1 .3
			SpinJitter		.3 .3 .2
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1
		End

		Geom	Widget_10
		
		Cdestroy	1
		CThresh 0.0001
		LifeSpan	20
		LifeSpanJitter	10

	End

End

Condition
	On 	Time
	Time 	0


	Event
		Ename	ToothHookupRear
		Type	local
		At	Origin
		
		BhvrOverride
			PositionOffset	0 10 -20.5
		End

	End

	Event
		Type	start 
		At	ToothHookupRear
		
		Bhvr	Behaviors/FadeOut.bhvr

		Part1	Powers/Claws/Clawflash.part
		Part3	:Embers.part
		Part4	:EmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part

		Part1	:fire.part
		Part1	:fire.part

		BhvrOverride
#			PositionOffset	5 9 -5
#			startJitter	1.3 1.3 1.3
			Scale		.6 .8 .6
			
			startcolor		85 75 55

			InitialVelocity		0.0 1.2 0.0			
			InitialVelocityJitter	0.8 0.25 0.8
			Spin			.1 .1 .3
			SpinJitter		.3 .3 .2
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1
		End

		Geom	Widget_10
		
		Cdestroy	1
		CThresh 0.0001
		LifeSpan	20
		LifeSpanJitter	10

	End

End


################################################################
##Bottem Nosecone Metal Bits 
##############################################


Condition
	
	Event
		Ename	NoseCone
		Type	local
		At	Origin
		
		BhvrOverride
			PositionOffset	0 17 13.5
		End

	End

	Event
		Ename	TopNoseCone
		Type	local
		At	Origin
		
		BhvrOverride
			PositionOffset	0 17 6.5
		End

	End

	Event
		Ename	TopNoseCone2
		Type	local
		At	Origin
		
		BhvrOverride
			PositionOffset	-2 17 6.5
		End

	End

	Event
		Ename	BottemNoseCone
		Type	local
		At	Origin
		
		BhvrOverride
			PositionOffset	2 11 10.5
		End

	End

#############################################################################
##NoseCone Smoke
###############################################
	 
	Event
		Type	local 
		At	origin

		BhvrOverride
			PositionOffset	0 11 6
		End

	End

	Event
		Type	local 
		At	origin

		BhvrOverride
			PositionOffset	0 14 14
		End

	End
End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start 
		At	BottemNoseCone
		
		Bhvr	Behaviors/FadeOut.bhvr

		Part1	Powers/Claws/Clawflash.part
#		Part3	:Embers.part
#		Part4	:EmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part

		Part1	:fire.part
		Part1	:fire.part

		BhvrOverride
#			PositionOffset	-11 11 0
			startJitter	1.3 1.3 1.3
			Scale		.7 .7 .7
			
			startcolor		85 75 55

			InitialVelocity		0.0 -0.15 0.0			
			InitialVelocityJitter	0.8 0.25 0.8
			Spin			.1 .1 .3
			SpinJitter		.3 .3 .2
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1
			FadeOutLength 		140
		End

		Geom	Widget_08
		
		Cdestroy	1
		CThresh 0.0001
		LifeSpan	20
		LifeSpanJitter	10

	End

End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start 
		At	BottemNoseCone
		
		Bhvr	Behaviors/FadeOut.bhvr

		Part1	Powers/Claws/Clawflash.part
		Part3	:Embers.part
		Part4	:EmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part

		Part1	:fire.part
		Part1	:fire.part

		BhvrOverride
#			PositionOffset	-11 11 0
			startJitter	1.3 1.3 1.3
			Scale		.7 .7 .7
			
			startcolor		85 75 55

			InitialVelocity		0.0 -0.15 0.0			
			InitialVelocityJitter	0.8 0.25 0.8
			Spin			.1 .1 .3
			SpinJitter		.3 .3 .2
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1
			FadeOutLength 		140
		End

		Geom	Metal_Flat01
		
		Cdestroy	1
		CThresh 0.0001
		LifeSpan	20
		LifeSpanJitter	30

	End

End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start 
		At	BottemNoseCone
		
		Bhvr	Behaviors/FadeOut.bhvr

		Part1	Powers/Claws/Clawflash.part
#		Part3	:Embers.part
#		Part4	:EmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part

		Part1	:fire.part
		Part1	:fire.part

		BhvrOverride
#			PositionOffset	-11 11 0
			startJitter	1.3 1.3 1.3
			Scale		.7 .7 .7
			
			startcolor		85 75 55

			InitialVelocity		0.0 -0.15 0.0			
			InitialVelocityJitter	0.8 0.25 0.8
			Spin			.1 .1 .3
			SpinJitter		.3 .3 .2
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1
			FadeOutLength 		140
		End

		Geom	Metal_Flat02
		
		Cdestroy	1
		CThresh 0.0001
		LifeSpan	20
		LifeSpanJitter	30

	End

End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start 
		At	BottemNoseCone
		
		Bhvr	Behaviors/FadeOut.bhvr

		Part1	Powers/Claws/Clawflash.part
		Part3	:Embers.part
		Part4	:EmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part

		Part1	:fire.part
		Part1	:fire.part

		BhvrOverride
#			PositionOffset	-11 11 0
			startJitter	1.3 1.3 1.3
			Scale		.7 .7 .7
			
			startcolor		85 75 55

			InitialVelocity		0.0 -0.15 0.0		
			InitialVelocityJitter	0.8 0.25 0.8
			Spin			.1 .1 .3
			SpinJitter		.3 .3 .2
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1
			FadeOutLength 		140
		End

		Geom	Metal_Flat01
		
		Cdestroy	1
		CThresh 0.0001
		LifeSpan	20
		LifeSpanJitter	30

	End

End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start 
		At	BottemNoseCone
		
		Bhvr	Behaviors/FadeOut.bhvr

		Part1	Powers/Claws/Clawflash.part
#		Part3	:Embers.part
#		Part4	:EmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part

		Part1	:fire.part
		Part1	:fire.part

		BhvrOverride
#			PositionOffset	-11 11 0
			startJitter	1.3 1.3 1.3
			Scale		.7 .7 .7
			
			startcolor		85 75 55

			InitialVelocity		0.0 -0.15 0.0		
			InitialVelocityJitter	0.8 0.25 0.8
			Spin			.1 .1 .3
			SpinJitter		.3 .3 .2
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1
			FadeOutLength 		140
		End

		Geom	Metal_Flat02
		
		Cdestroy	1
		CThresh 0.0001
		LifeSpan	20
		LifeSpanJitter	30

	End

End

##################################################################
####Top Nosecone Metal Bits 
################################################
#
Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start 
		At	TopNoseCone
		
		Bhvr	Behaviors/FadeOut.bhvr

		Part1	Powers/Claws/Clawflash.part
#		Part3	:Embers.part
#		Part4	:EmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part

		Part1	:fire.part
		Part1	:fire.part

		BhvrOverride
#			PositionOffset	-6 18 -4
			startJitter	1.3 1.3 1.3
			Scale		.7 .7 .7
			
			startcolor		85 75 55

			InitialVelocity		0.0 0.45 0.0		
			InitialVelocityJitter	0.8 0.25 0.8
			Spin			.1 .1 .3
			SpinJitter		.3 .3 .2
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1
			FadeOutLength 		140
		End

		Geom	Widget_08
		
		Cdestroy	1
		CThresh 0.0001
		LifeSpan	20
		LifeSpanJitter	10

	End

End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start 
		At	TopNoseCone
		
		Bhvr	Behaviors/FadeOut.bhvr

		Part1	Powers/Claws/Clawflash.part
		Part3	:Embers.part
		Part4	:EmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part

		Part1	:fire.part
		Part1	:fire.part

		BhvrOverride
#			PositionOffset	-6 18 -4
			startJitter	1.3 1.3 1.3
			Scale		.7 .7 .7
			
			startcolor		85 75 55

			InitialVelocity		0.0 0.45 0.0		
			InitialVelocityJitter	0.8 0.25 0.8
			Spin			.1 .1 .3
			SpinJitter		.3 .3 .2
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1
			FadeOutLength 		140
		End

		Geom	Metal_Flat01
		
		Cdestroy	1
		CThresh 0.0001
		LifeSpan	20
		LifeSpanJitter	30

	End

End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start 
		At	TopNoseCone2
		
		Bhvr	Behaviors/FadeOut.bhvr

		Part1	Powers/Claws/Clawflash.part
#		Part3	:Embers.part
#		Part4	:EmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part

		Part1	:fire.part
		Part1	:fire.part

		BhvrOverride
#			PositionOffset	-6 18 -4
			startJitter	1.3 1.3 1.3
			Scale		.7 .7 .7
			
			startcolor		85 75 55

			InitialVelocity		0.0 0.85 0.0		
			InitialVelocityJitter	0.8 0.25 0.8
			Spin			.1 .1 .3
			SpinJitter		.3 .3 .2
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1
			FadeOutLength 		140
		End

		Geom	Metal_Flat02
		
		Cdestroy	1
		CThresh 0.0001
		LifeSpan	20
		LifeSpanJitter	30

	End

End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start 
		At	TopNoseCone2
		
		Bhvr	Behaviors/FadeOut.bhvr

		Part1	Powers/Claws/Clawflash.part
		Part3	:Embers.part
		Part4	:EmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part

		Part1	:fire.part
		Part1	:fire.part

		BhvrOverride
#			PositionOffset	-6 18 0
			startJitter	1.3 1.3 1.3
			Scale		.7 .7 .7
			
			startcolor		85 75 55

			InitialVelocity		0.0 0.45 0.0		
			InitialVelocityJitter	0.8 0.25 0.8
			Spin			.1 .1 .3
			SpinJitter		.3 .3 .2
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1
			FadeOutLength 		140
		End

		Geom	Metal_Flat01
		
		Cdestroy	1
		CThresh 0.0001
		LifeSpan	20
		LifeSpanJitter	30

	End

End

Condition
	On 	Time
	Time 	0
	Repeat	1

	Event
		Type	start 
		At	TopNoseCone
		
		Bhvr	Behaviors/FadeOut.bhvr

		Part1	Powers/Claws/Clawflash.part
#		Part3	:Embers.part
#		Part4	:EmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part

		Part1	:fire.part
		Part1	:fire.part

		BhvrOverride
#			PositionOffset	-6 18 0
			startJitter	1.3 1.3 1.3
			Scale		.7 .7 .7
			
			startcolor		85 75 55

			InitialVelocity		0.0 0.85 0.0		
			InitialVelocityJitter	0.8 0.25 0.8
			Spin			.1 .1 .3
			SpinJitter		.3 .3 .2
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1
			FadeOutLength 		140
		End

		Geom	Metal_Flat02
		
		Cdestroy	1
		CThresh 0.0001
		LifeSpan	20
		LifeSpanJitter	30

	End

End

################################################################
##Nosecone Metal Bits 
##############################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start 
		At	NoseCone
		
		Bhvr	Behaviors/FadeOut.bhvr

		Part1	Powers/Claws/Clawflash.part
		Part3	:Embers.part
		Part4	:EmbersBroad.part
		Part3	:Embers.part
		Part4	:EmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part

		Part1	:fire.part
		Part1	:fire.part

		BhvrOverride
#			PositionOffset	-15 17 -2
			startJitter	1.3 1.3 1.3
			Scale		.7 .7 .7
			
			startcolor		85 75 55

			InitialVelocity		0.15 0.45 0.0		
			InitialVelocityJitter	0.1 0.2 0.2
			Spin			.1 .1 .3
			SpinJitter		.3 .3 .2
			Drag			0.03
			physics 		2
			physRadius 		0.15
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1
			FadeOutLength 		140
		End

		Geom	Widget_03
		
		Cdestroy	1
		CThresh 0.0001
		LifeSpan	70
		LifeSpanJitter	30

	End

End

Condition
	On 	Time
	Time 	0
	repeat	2
	
	Event
		Type	start 
		At	NoseCone
		
		Bhvr	Behaviors/FadeOut.bhvr

		Part1	Powers/Claws/Clawflash.part
#		Part3	:Embers.part
#		Part4	:EmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part

		Part1	:fire.part
		Part1	:fire.part

		BhvrOverride
#			PositionOffset	-15 17 -2
			startJitter	1.3 1.3 1.3
			Scale		.27 .27 .5
			
			startcolor		85 75 55

			InitialVelocity		0.15 0.45 0.0		
			InitialVelocityJitter	0.1 0.2 0.2
			Spin			.1 .1 .3
			SpinJitter		.3 .3 .2
			Drag			0.03
			physics 		2
			physRadius 		1
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1
			FadeOutLength 		140
		End

		Geom	Widget_11
		
		Cdestroy	1
		CThresh 0.0001
		LifeSpan	20
		LifeSpanJitter	20

	End

End

Condition
	On 	Time
	Time 	0
	repeat	2
	
	Event
		Type	start 
		At	NoseCone
		
		Bhvr	Behaviors/FadeOut.bhvr

		Part1	Powers/Claws/Clawflash.part
#		Part3	:Embers.part
#		Part4	:EmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part

		Part1	:fire.part
		Part1	:fire.part

		BhvrOverride
#			PositionOffset	-15 17 -2
			startJitter	1.3 1.3 1.3
			Scale		.5 .8 .5
			
			startcolor		85 75 55

			InitialVelocity		0.15 0.45 0.0		
			InitialVelocityJitter	0.1 0.2 0.2
			Spin			.1 .1 .3
			SpinJitter		.3 .3 .2
			Drag			0.03
			physics 		2
			physRadius 		1
			physGravity 		.7
			physRestitution 	0.15
			physSFriction 	 	0.8
			physDFriction 	 	0.8
			Gravity 		.05
			Physdebris		1
			FadeOutLength 		140
		End

		Geom	Widget_02
		
		Cdestroy	1
		CThresh 0.0001
		LifeSpan	70
		LifeSpanJitter	30

	End

End

End
