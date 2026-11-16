#########################################################
##	cot_waterfall_med.fx
##
FxInfo

#LifeSpan	100

Input  
	InpName	Origin
End


Input  
	InpName	Target
End

Input  
	InpName	WepR
End


###########################################################
##################################
##################################

Condition
	On 	Time
	Time 	0

	Event
		
		Type	Local
		At	C_T2_R
		bhvr	:ThornGrowRotR.bhvr
		Geom	Thorn4
		lifespan	17
	End

#	Event
#		
#		Type	Local
#		At	C_T2_L
#		bhvr	:ThornGrowRotR.bhvr
#		Geom	Thorn4
#		lifespan	17
#	End
#
end

Condition
	On 	Time
	Time 	1

	
	Event
		
		Type	Local
		At	C_T3_R
		bhvr	:ThornGrowRotR.bhvr
		Geom	Thorn4
		lifespan	17
	End
#	Event
#		
#		Type	Local
#		At	C_T3_L
#		bhvr	:ThornGrowRotR.bhvr
#		Geom	Thorn4
#		lifespan	17
#	End
end

Condition
	On 	Time
	Time 	2

	
	Event
		
		Type	Local
		At	C_Hind_UlegR
		bhvr	:ThornGrowRotR.bhvr
		Geom	Thorn4
		lifespan	17
	End
#	Event
#		
#		Type	Local
#		At	C_Hind_UlegL
#		bhvr	:ThornGrowRotR.bhvr
#		Geom	Thorn4
#		lifespan	17
#	End
end

Condition
	On 	Time
	Time 	3

	
	Event
		
		Type	Local
		At	C_Hind_LlegR
		bhvr	:ThornGrowRotR.bhvr
		Geom	Thorn4
		lifespan	17
	End
#	Event
#		
#		Type	Local
#		At	C_Hind_LlegL
#		bhvr	:ThornGrowRotR.bhvr
#		Geom	Thorn4
#		lifespan	17
#	End
end
#################


Condition
	On 	Time
	Time 	3

	Event
		
		Type	Local
		At	C_T1_R
		part1	:thorndust.part
		lifespan	32
		pother	C_F2_R
	End
	Event
		
		Type	Local
		At	C_T2_R
		part1	:thorndust.part
		lifespan	32
		pother	C_T1_R
	End
	Event
		
		Type	Local
		At	C_T3_R
		part1	:thorndust.part
		lifespan	32
		pother	C_T2_R
	End
	Event
		
		Type	Local
		At	C_Hind_UlegR
		part1	:thorndust.part
		lifespan	32
		pother	C_T3_R
	End
	Event
		
		Type	Local
		At	C_Hind_LlegR
		part1	:thorndust.part
		lifespan	32
		pother	C_Hind_UlegR
	End
End




#	Event
#		
#		Type	Local
#		At	C_F1_R
#		bhvr	:ThornGrowRotR.bhvr
#		Geom	Thorn4
#	End
#	Event
#		
#		Type	Local
#		At	C_F2_R
#		bhvr	:ThornGrowRotR.bhvr
#		Geom	Thorn4
#	End
#	Event
#		
#		Type	Local
#		At	C_T1_R
#		bhvr	:ThornGrowRotR.bhvr
#		Geom	Thorn4
#	End

###

#	Event
#		
#		Type	Local
#		At	C_F1_L
#		bhvr	:ThornGrowRotR.bhvr
#		Geom	Thorn4
#	End
#	Event
#		
#		Type	Local
#		At	C_F2_L
#		bhvr	:ThornGrowRotR.bhvr
#		Geom	Thorn4
#	End
#	Event
#		
#		Type	Local
#		At	C_T1_L
#		bhvr	:ThornGrowRotR.bhvr
#		Geom	Thorn4
#	End