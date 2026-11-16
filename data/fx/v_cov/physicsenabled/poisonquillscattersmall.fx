#########################################################
##	template

FxInfo

#########################################################

Condition
	On Time 
	Time 0
	
	Event
		Type 	Start
		At	Origin
		Bhvr	:SparkOUT.bhvr
		BhvrOverride
			SpinJitter		2 2 2
			PhysScale		2 2 2
			Scale			.35 .35 .35
			InitialVelocity		0.0 .25 0.0
			InitialVelocityJitter	0.2 0.2 0.2
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			Physdebris	1

		End
		CThresh 	0.000001

		Geom	quill01
		Geom	quill06
		Geom	quill03
#		Geom	quill04
		#Part	:PoisonQuill.part

		Lifespan	60
	End
End	

End	

	