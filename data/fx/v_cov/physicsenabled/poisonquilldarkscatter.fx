#############################################################
## HEADER
#############################################################

FxInfo

ClampMinScale 0.75 0.75 0.75
ClampMaxScale 1.25 1.25 1.25

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type 	Start
		At	Origin
		Bhvr	:SparkOUT.bhvr
		BhvrOverride
			InitialVelocity		0.1 0.5 0.2
			InitialVelocityJitter	0.4 0.4 0.4
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
		End
		CThresh 	0.000001
		Part	:PoisonQuillDark.part
		Lifespan 60
	End
End

#############################################################

End