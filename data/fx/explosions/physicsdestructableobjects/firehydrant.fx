#########################################################
##	onfire
##
FxInfo  

LifeSpan	100

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime1
		Type	Local 
		At	chest
		
		#Sound	telekinetic3 80 30 .9

	End

End

#Condition
#
#	Event
#		Type	start 
#		At	chest
#		
#		BhvrOverride
#			Drag			0.006
#			physics 		1
#			physRadius 		0.8
#			physGravity 		.9
#			physRestitution 	0.5
#			physSFriction 	 	0.3
#			physDFriction 	 	0.5
#		End
#
#		Geom	_Firehydrant
#
#	End
#
#End


End			