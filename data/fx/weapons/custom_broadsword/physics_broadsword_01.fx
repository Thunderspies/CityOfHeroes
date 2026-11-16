#########################################################
## Physics GEO - Broadsword 01
#########################################################

FxInfo

#Lifespan 300

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start 
		At	WepR
		BhvrOverride
			PyrRotateJitter		0 0 0
			InitialVelocity		0 0 0
			InitialVelocityJitter	0.0 0.125 0.0
			Spin			0.0 0.0 0.0625
			SpinJitter		0.0 0.0 0.03125
			physics 		1
			physRadius 		0.8
			physGravity 		0.9
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
			FadeOutLength		90
			PhysDensity		1
		End
		Geom	GEO_WepR_Sword_09
		CEvent 	V_COV\PhysicsEnabled\ScrapeSparks.fx
		CThresh 0.0001
		Lifespan	120
		LifespanJitter	30
	End
End

#########################################################

End		