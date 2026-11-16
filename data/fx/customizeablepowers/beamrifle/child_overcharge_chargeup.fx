#############################################################
## Child_SingleShot.fx
#############################################################

FxInfo

Lifespan 300

#############################################################
Condition
	On 	Time
	Time 	0

	Event
		EName 	LocknarFadeIn
		Type	Local
		At	Origin
		#Bhvr	:ChargeUpBall.bhvr
		BhvrOverride
			#PositionOffset		-0.25 0.2 1.6
			Scale			1 1 1
			ScaleRate		1.5 1.5 1.5
 			EndScale		5 5 5
		End
		#Geom	SeedInfluenceRad
		#Part	:Ring_Smoke_01.part
		Part	:ChargeUpStar.part
		Part	:ChargeUpClouds.part
		Lifespan 60
	End

#	Event
#		EName 	LocknarFadeIn2
#		Type	Local
#		At	Origin
#		Bhvr	:ChargeUpBall.bhvr
#		BhvrOverride
#			#PositionOffset		-0.25 0.2 1.6
#			#PyrRotate 		180 180 180
#			Spin			0.3 0.8 0.3
#			SpinJitter		0.1 0.1 0.1
#			Scale			1 1 1
#			ScaleRate		1.01 1.01 1.01
# 			EndScale		1.2 1.2 1.2
#		End
#		Geom	SeedInfluenceRad
#		#Part	:ChargeUp_BallEdges.part
#		Lifespan 60
#	End
End

#############################################################

End