#########################################################
##	template

FxInfo

Lifespan 50

#########################################################

Condition
	On	Cycle
	Time	3
	
	Event
		EName	Shells
		Type 	Start
		At	Origin
		
		bhvr	V_COV\Powers\Thugs\B_Physics_ShellCasings.bhvr
		Geom	GEO_ShellCasing
		Lifespan 50
		CThresh 0.000001

	End
End

End		
		
		
		
#		Bhvr	V_COV\PhysicsEnabled\SparkOUT.bhvr

#		BhvrOverride
#			InitialVelocity		0.1 0.2 0.05	
#			InitialVelocityJitter	0.1 0.1 0.1
#			physRestitution 	0.5
#			physSFriction 	 	0.5
#			physDFriction 	 	0.5
#		End
#
#		#CThresh 	0.000001
#		#Cevent		:GunShellBounceSound.fx
#
#		Part	V_COV\PhysicsEnabled\PistolShell.part
#
#		Lifespan	25
#	End
#End	
#
#End	
#
	