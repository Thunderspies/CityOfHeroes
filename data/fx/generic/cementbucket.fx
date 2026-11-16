
FxInfo

#####  Cement Bucket   #######################################

Condition
	On 	Time  
	Time 	0

	Event
		EName	Prime					#existing event name, not totally clear
		Type	Local
		At	FootR					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		Geom	FootR_CementBucket			#call to geometry fx
	End
End

End		