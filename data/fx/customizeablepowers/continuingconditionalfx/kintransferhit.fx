#############################################################
## Header
#############################################################

FxInfo

Lifespan 100

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	HitSwirlNode
		Type	Posit
		At	Root
		Geom	RootToChestAdjustment
		Lifespan 30
	End

	Event
		ENAME	HitSwirl1
		Type	Local
		At	HitSwirlNode
		AltPiv	1
		part	:KineticTransferHitRingLrg01.part
		Lifespan 30
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	SpinNode
		Type	Local
		At	Hips
	End

	Event
		ENAME	Swirl1
		Type	Local
		At	SpinNode
		Bhvr	POWERS/Kinetics/KineticSpin1.bhvr
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Lifespan 20
	End

	Event
		ENAME	Swirl2
		Type	Local
		At	SpinNode
		Bhvr	POWERS/Kinetics/KineticSpin2.bhvr
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Lifespan 20
	End

	Event
		ENAME	Swirl3
		Type	Local
		At	SpinNode
		Bhvr	POWERS/Kinetics/KineticSpin3.bhvr
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Lifespan 20
	End

	Event
		ENAME	Glows1
		Type	Local
		At	SpinNode
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:KineticExpandingGlows01.part
		part	:KineticExpandingRings01.part
		Lifespan 20
	End

	Event
		ENAME	Flyaways1
		Type	Start
		At	Swirl1
		part	:KineticSwirlFlyAway01.part
		Lifespan 20
	End

	Event
		ENAME	Flyaways2
		Type	Start
		At	Swirl2
		part	:KineticSwirlFlyAway02.part
		Lifespan 20
	End
End



End