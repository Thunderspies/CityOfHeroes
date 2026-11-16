
FxInfo

Flags InheritAnimScale

#####  BodyBag  #######################################

Condition
	On 	Time						#conditions specify when fx happens, on death...time, hit, etc.    
	Time 	0

	Event
		EName	Prime					#Event Name, can be any name I wish, used when events refer to another event
		Type	Local
		At	Mystic					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		Anim	FX_BodyBag				#Anim refers to Entype filename
	End
	
End

End		