#############################################################
## HEADER
#############################################################

FxInfo

Flags InheritAnimScale

LifeSpan 300

######  uprooted branch  #####################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Sound stumppickup 80 80 .96
	End
End

Condition
	On 	Time
	Time 	32


	Event
		Type	local
		At	WepR
		bhvr	behaviors\Stump_Grab.bhvr
		Geom    TreeStump

		Lifespan	60

	End

End


######  Dirt from ground #####################

Condition
	On 	Time
	Time 	32

	Event
		EName	concrete1
		Type	start
		At	Root
		#bhvr	behaviors\SlightQuake.bhvr
		Part1	:Crack.part

	End

End


Condition
	On 	Time
	Time 	40

	Event
		EName 	GroundBurst
		Type	Start
		At	Mystic
		Part	:superStrengthDust1.part
		part	:superStrengthDust2.part
		Part	:HitRocks.part
		#Part	:HitRocks.part

		LifeSpan 1
	End


End


###### Projectile Root  #################################

Condition
	On 	Time
	Time 	90


	Event
		EName 	Prime
		Type	Start
		At	Mystic	#WepR
		Geom    TreeStump
		bhvr	behaviors\stump_toss.bhvr
		Magnet	Target
		LookAt	Target


	End


End

Condition
	On	PrimeHit

	Repeat	35
	RepeatJitter	5

	Event
		Type	start
		At	T_chest
		Geom	woodchip
		Bhvr	Behaviors\StartOffset.bhvr
		BhvrOverride
			StartColor		140 140 140
			Scale			.1 .1 .1
			ScaleRate		1 1 1
			EndScale		.1 .1 .1
			InitialVelocity		.0 0.4 .0
			InitialVelocityJitter	0.2 0.2 0.2
			PyrRotateJitter		180 180 180
			Drag			0.004
			physics 		1
			physRadius 		.8
			physGravity 		1
			physRestitution 	0.7
			physSFriction 	 	0.4
			physDFriction 	 	0.7
			FadeOutLength		1110
			SpinJitter		.1 .1 .1
		End
		#Part1	Powers\FireControl\BurningMeteor.part
		Lifespan	1120
		LifespanJitter	130
	End

	Event
		Type	start
		At	T_chest
		Geom	woodchip
		Bhvr	Behaviors\StartOffset.bhvr
		BhvrOverride
			StartColor		140 140 140
			Scale			.05 .05 .5
			ScaleRate		1 1 1
			EndScale		.05 .05 .05
			InitialVelocity		.0 0.4 .0
			InitialVelocityJitter	0.2 0.2 0.2
			PyrRotateJitter		180 180 180
			Drag			0.004
			physics 		1
			physRadius 		.8
			physGravity 		1
			physRestitution 	0.7
			physSFriction 	 	0.4
			physDFriction 	 	0.7
			FadeOutLength		1110
			SpinJitter		.1 .1 .1
		End
		#Part1	Powers\FireControl\BurningMeteor.part
		Lifespan	1120
		LifespanJitter	130
	End
End

Condition
	On	PrimeHit


	Event
		Type	Start
		At	T_Chest
		Part	:superStrengthDustHit1.part
		part	:superStrengthDust2Hit.part
		Part	:HitRocks.part
		Part	:HitRocks.part
		Sound stumphit2 80 80 .8
		LifeSpan 20

	End

	Event
		Type	Start
		At	T_Chest

		Part	Explosions\WoodExplosion\SawDust01.part
		Part	Explosions\WoodExplosion\SawDust02.part
		Part	Explosions\WoodExplosion\SawdustLarge.part
#		Part	Explosions\WoodExplosion\SawDust01.part
#		Part	Explosions\WoodExplosion\SawDust02.part
#		Part	Explosions\WoodExplosion\SawdustLarge.part

		LifeSpan 5

	End

	Event
		Type	StartPositOnly
		At	T_Chest

		BHVR   :StumpHitLeft.bhvr
		Geom   TreeStump_Piece02
		#Geom   Bottle1

		LifeSpan 2000

	End

	Event
		Type	StartPositOnly
		At	T_Chest

		BHVR   :StumpHitRight.bhvr
		Geom   TreeStump_Piece01
		#Geom   Bottle1
		LifeSpan 2000

	End


	Event
		Ename	Prime
		Type	Destroy
	End

End

End