
FxInfo

Lifespan	180

Flags InheritAnimScale

#####  BodyBag  #######################################

Condition
	On 	Time						#conditions specify when fx happens, on death...time, hit, etc.    
	Time 	0
	
	
	Event
		EName 	RopeStart
		Type	Start
		At	Root
		
		bhvr	V_COV\Powers\Paramilitary\Paramilitary_B_RopeStart.bhvr
#		Geom	Mystic_LapTopTable #Testing_Target
	End
End



Condition
	On 	Time						#conditions specify when fx happens, on death...time, hit, etc.    
	Time 	0

	Event
		EName	Rope					#Event Name, can be any name I wish, used when events refer to another event
		Type	Start
		At	Root					#location for geometry placement, can be any bone name, "ROOT" or "ORIGIN"
		
		bhvr	V_COV\Powers\Paramilitary\Paramilitary_B_RopeFade.bhvr
		Anim	FX_RappelRope				#Anim refers to Entype filename
#		Geom	Testing_Target
		Lifespan 70
	End
	
End

Condition
	On 	Time						#conditions specify when fx happens, on death...time, hit, etc.    
	Time 	1

	Event
		EName 	RopeTop
		Type	Local
		At	BendMystic
		BhvrOverride
			behavior
			positionOffset		-1.5 0 0
			scale			3 3 3
		End
		
#		Geom	Testing_Target
	End
End

Condition
	On 	Time						#conditions specify when fx happens, on death...time, hit, etc.    
	Time 	2
	
	Event
		EName 	RopeStretch
		Type	Start
		At	RopeStart
		
		bhvr	V_COV\Powers\Paramilitary\Paramilitary_B_RopeStretch.bhvr
		Geom	GEO_RopeStretch
		LookAt	RopeTop
		Lifespan 70
	End
End


End		