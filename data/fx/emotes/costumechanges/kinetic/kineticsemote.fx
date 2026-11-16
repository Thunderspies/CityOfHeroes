#########################################################
##	template

FxInfo




#Lifespan 200

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	15


	Event
		Type	Local
		At 	Origin
		Sound kinetic2 80 80 .85
	End
End

####################### Force ###########################

Condition
	On	Time
	Time	30

	Event
		Ename	GravitationalForce
		Type	Local
		At	Hips

		BhvrOverride
			PhysForceType		CCWSwirl
			PhysForceCentripetal	-2
			PhysForceRadius		10
			PhysForcePower		1000
			PhysForcePowerJitter	25
		End

		#Lifespan	150

	End

	Event
		Ename	LevitationForce
		Type	Local
		At	Root

		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		11
			PhysForcePower		150
			PhysForcePowerJitter	5
		End

		#Lifespan	120

	End
End
#########################################################
#################### BODY GLOW 1 ########################
#########################################################


Condition
	On	Cycle
	Time	60

	Event
		ENAME	ScaleNode
		Type	Local
		At	Hips
		Bhvr	:KineticScale.bhvr
		Sound Kin2_loop 80 80 .5
		Lifespan 50

	End

#	Event
#		ENAME	Swirl1
#		Type	Local
#		At	ScaleNode
#		part	:KinecTestSwirl02.part
#		part	:KinecTestSwirl01.part
#		Bhvr	:KineticSpin1.bhvr
#		Lifespan 50
#
#	End
#
#	Event
#		ENAME	Swirl2
#		Type	Local
#		At	ScaleNode
#		part	:KinecTestSwirl02.part
#		part	:KinecTestSwirl01.part
#		Bhvr	:KineticSpin2.bhvr
#		Lifespan 50
#
#	End

	Event
		ENAME	Swirl3
		Type	Local
		At	ScaleNode
		part	:KinecTestSwirl02.part
		part	:KinecTestSwirl01.part
		Bhvr	:KineticSpin3.bhvr
		Lifespan 50

	End
#
#	Event
#		ENAME	Glows1
#		Type	Local
#		At	ScaleNode
#		part	:KineticExpandingGlowsRed01.part
#		part	:KineticExpandingRings01.part
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		Lifespan 50
#
#	End
#
#	Event
#		ENAME	Flyaways1
#		Type	Start
#		At	Swirl1
#		part	:KineticSwirlFlyAway01.part
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		Lifespan 50
#
#	End
#End
#
#Condition
#	On	Cycle
#	Time	48
#
#	Event
#		ENAME	Pulse1
#		Type	Posit
#		At	Hips
#		part	:KineticOval01.part
#		Bhvr	Behaviors/GenericParticleFade.bhvr
#		Lifespan 45
#
#	End


End

