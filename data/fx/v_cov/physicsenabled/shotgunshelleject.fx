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
			PhysScale		2.0 2.0 5
			Scale			1.0 1.0 1.0
			Spinjitter		.25 .25 .25
			InitialVelocity		0.0 0.3 0.0	
			InitialVelocityJitter	0.15 0.15 0.15
			physRestitution 	0.5
			physSFriction 	 	0.5
			physDFriction 	 	0.5
			PhysDebris		1
		End

		#CThresh 	0.000001
		#Cevent		:GunShellBounceSound.fx

		Geom	ShotgunShell
		#Part	:ShotgunShell.part

		Lifespan	1
	End
End	

End	

	