#########################################################
##	LOOKS BAD - Do Not use
##
FxInfo

LifeSpan	1220

Condition

	Event
		Type	start
		At	root
		
		Part	:DiffuseWaterBurst.part
		Part	:DiffuseWaterStreaks.part
		Part	:DiffuseWaterMistStreaks.part

		Lifespan	1220
		LifespanJitter	30
	End
	
	Event
		Ename	LevitationForce2
		Type	Start
		At	Root
		
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		5
			PhysForcePower		350
			PhysForcePowerJitter	50
		End

	End
	
	Event
		Ename	ScatterForce
		Type	Start
		At	Root
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		7
			PhysForcePower		40
			PhysForcePowerJitter	25
		End

	End



End

Condition

	Event
		Type	start
		At	root
		Geom	Broken_Fx_Firehydrant
		
		Bhvr	V_COV\PhysicsEnabled\MetalChunkScatter.bhvr
		BhvrOverride
			StartJitter		0 0 0		
			scale			1 1 1

			InitialVelocity		0 0.35 0
			InitialVelocityJitter	0.25 0.1 0.25

#			PyrRotateJitter		180 180 180

			Drag			0.006
			physics 		1
			physRadius 		0.8
			physGravity 		.4
			physRestitution 	0.1
			physSFriction 	 	0.3
			physDFriction 	 	0.3
			FadeOutLength		510

			PhysDebris		3
		End

		CThresh 0.0001

		Lifespan	2
	End

End
Condition
	On 	Time
	Time 	0

	Event
		Ename	Hookup
		Type	start 
		At	root
		bhvr	Behaviors/FadeOut.bhvr
		part1	:DiffuseWaterFountainBodyBubblerL2.part
		part3	:DiffuseWaterFountainMistBodyBubblerL2.part
		part4	:DiffuseWaterBubble2BubblerL2.part
		part5	:DiffuseWaterBubbleBody2BubblerL2.part
		
		LifeSpan	1135		
	End
End


Condition
	On 	Time
	Time 	14

	Event
		Type	start 
		At	Hookup

		bhvr	Behaviors/FadeOut.bhvr
		Part3	:DiffuseFlatWaterXtraLargea.part
		Part4	:DiffuseCenterWaterXtraLargea.part
		
	End
	

End

End			