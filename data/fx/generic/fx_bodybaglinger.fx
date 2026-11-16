
FxInfo

Flags InheritAnimScale

#####  BodyBag  #######################################

Lifespan	900

Condition
	On 	Time						#conditions specify when fx happens, on death...time, hit, etc.    
	Time 	0

	Event
		EName	Prime					#Event Name, can be any name I wish, used when events refer to another event
		Type	Start					#Local, Start - local is local to bone, start begins at start frame location
		At	Root					#location for geometry placement, can be any bone name, "ROOT" - world relative, or "ORIGIN" - Hips relative
		Anim	FX_BodyBagLinger			#Anim refers to Entype filename
	End
	
End

End		