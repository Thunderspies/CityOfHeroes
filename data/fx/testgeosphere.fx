#############################################################
## FX System Name
#############################################################

FxInfo

## TEST SPHERE #######################################################

Condition
	On 	Time
	Time	0

	Event
		EName	Prime
		Type	Local
		At	Mystic
		Bhvr	Behaviors\null.bhvr
		BhvrOverride
			Scale	0.5 0.5 0.5
		End
		Geom	GEO_FX_TestSphere
		#Lifespan	60
	End
End

#############################################################

End