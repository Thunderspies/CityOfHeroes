#########################################################
##	template

FxInfo


Flags InheritAnimScale

LifeSpan 300

######## arrow in hand   ###########################

Condition
	On	Time
	Time	30

	Event	
		Type	Local
		At	WepR
		Sound arrowout2 50 50 .25
	End

	Event	
		ename	Scalar
		Type	Local
		At	WepR
		BhvrOverride
			PositionOffset  -.3 0.08 .2
			Scale		.6 .6 .6
		End

	End

	Event	
		ename	Scalar2
		Type	Local
		At	WepR
		BhvrOverride
			PositionOffset  1 0.08 .6
		End
#		geom	testing_targetb

	End

End


Condition
	On 	Time
	Time 	30

	
	Event
		Type	Local
		At	Scalar
		Bhvr	:HandRotate.bhvr
		Geom	NormalArrow
		LifeSpan 16
	End
	
	Event
		Type	Local
		At	Scalar
		Bhvr	:HandRotate1.bhvr
		Geom	NormalArrow
		LifeSpan 16
	End
	
	Event
		Type	Local
		At	Scalar
		Bhvr	:HandRotate2.bhvr
		Geom	NormalArrow
		LifeSpan 16
	End
	
	Event
		Type	Local
		At	Scalar
		Bhvr	:HandRotate3.bhvr
		Geom	NormalArrow
		LifeSpan 16
	End
	
	Event
		Type	Local
		At	Scalar
		Bhvr	:HandRotate4.bhvr
		Geom	NormalArrow
		LifeSpan 16
	End

End	

#########  Projectile Target Set Up  ###########

Condition
	On	Time
	Time	31

	Event
		Ename	projectileTarget
		Type	local
		At	T_Chest
		BHVR	:Scale.bhvr
		Geom	ConeDummy
		 Geom	ConeDummy2
		 Geom	ConeDummy3
		 Geom	ConeDummy4
		 
	End

	Event
		Ename	1
		Type	local
		At	projectileTarget
		AltPiv 1
		Geom	ConeDummy
		 Geom	ConeDummy2
		 Geom	ConeDummy3
		 Geom	ConeDummy4
		 
	End

	Event
		Ename	2
		Type	start
		At	projectileTarget
		AltPiv	2	
		Geom	ConeDummy
		 Geom	ConeDummy2
		 Geom	ConeDummy3
		 Geom	ConeDummy4
	End
	
	Event
		Ename	3
		Type	start
		At	projectileTarget
		AltPiv	3	
		Geom	ConeDummy
		 Geom	ConeDummy2
		 Geom	ConeDummy3
		 Geom	ConeDummy4
		
	End
	
	Event
		Ename	4
		Type	start
		At	ProjectileTarget
		AltPiv	4
		Geom	ConeDummy
		 Geom	ConeDummy2
		 Geom	ConeDummy3
		 Geom	ConeDummy4
		
	End
	
	Event
		Ename	5
		Type	start
		At	ProjectileTarget
		AltPiv	5	
		Geom	ConeDummy
		 Geom	ConeDummy2
		 Geom	ConeDummy3
		 Geom	ConeDummy4
		
	End
	
	Event
		Ename	6
		Type	start
		At	ProjectileTarget
		AltPiv	6	
		Geom	ConeDummy
		 Geom	ConeDummy2
		 Geom	ConeDummy3
		 Geom	ConeDummy4
	End

End



######## arrow projectile  ##########################
Condition
	On 	Time
	Time 	46

	
	Event
		EName 	prime
		Type	start
		At	Scalar2
		Bhvr	:CrossbowConeProjectileSmall.bhvr
		Geom	aNormalArrow

		Magnet	target
		Lookat	target
		#LifeSpan 8

	End

	
	Event
		EName 	p
		Type	start
		At	Scalar2
		Bhvr	:CrossbowConeProjectileSmall.bhvr
		Geom	aNormalArrow
		Sound bowtwangcone2 100.0 100.0 .7

		Magnet	6
		Lookat	Scalar2
		LifeSpan 15

	End

	
	Event
		EName 	p5
		Type	start
		At	Scalar2
		Bhvr	:CrossbowConeProjectileSmall.bhvr
		Geom	aNormalArrow
		#Sound 	Crossbow1 100.0 30.0 .7

		Magnet	5
		Lookat	Scalar2
		LifeSpan 15
	End
	
	Event
		EName 	p4
		Type	start
		At	Scalar2
		Bhvr	:CrossbowConeProjectileSmall.bhvr
		Geom	aNormalArrow
		#Sound 	Crossbow1 100.0 30.0 .7

		Magnet	4
		Lookat	Scalar2
		LifeSpan 15

	End
	
	Event
		EName 	p3
		Type	start
		At	Scalar2
		Bhvr	:CrossbowConeProjectileSmall.bhvr
		Geom	aNormalArrow
		#Sound 	Crossbow1 100.0 30.0 .7

		Magnet	3
		Lookat	Scalar2
		LifeSpan 15

	End
	
	Event
		EName 	p2
		Type	start
		At	Scalar2
		Bhvr	:CrossbowConeProjectileSmall.bhvr
		Geom	aNormalArrow
		#Sound 	Crossbow1 100.0 30.0 .7

		Magnet	2
		Lookat	Scalar2
		LifeSpan 15

	End
	
	Event
		EName 	p1
		Type	start
		At	Scalar2
		Bhvr	:CrossbowConeProjectileSmall.bhvr
		Geom	aNormalArrow
		#Sound 	Crossbow1 100.0 30.0 .7

		Magnet	1
		Lookat	Scalar2
		LifeSpan 15

	End
End

Condition
	On 	PrimeHit

	Event
		Ename 	All
		Type	Destroy
	End

End

End		