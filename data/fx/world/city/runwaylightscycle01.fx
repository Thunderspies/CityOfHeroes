#########################################################
##	SStreetSteeam01
##
FxInfo
	
Condition
	On 	Time
	Time 	0

	Event
		EName 	lightgeom
		Type	Local 
		At	origin
		Geom	RunwayLightOrigin
								
	End


	Event
		Type	Local 
		At	lightgeom
				
		altpiv	2
		BHVR	:RunwaylightOffset1.bhvr
		Part	:LargeRedLightPulsing.part
		Part	:LargeRedLightPulsing.part
				
	End

End

#########################################################################
	
Condition
	On 	Time
	Time 	5



	Event
		Type	Local 
		At	lightgeom
				
		BHVR	:RunwaylightOffset.bhvr	
		altpiv	2
		
		Part	:LargeRedLightPulsing.part
		Part	:LargeRedLightPulsing.part
				
	End
	
End

#########################################################################
	
Condition
	On 	Time
	Time 	10


	Event
		Type	Local 
		At	lightgeom
				
		BHVR	:RunwaylightOffset3.bhvr	
		altpiv	2
		
		Part	:LargeRedLightPulsing.part
		Part	:LargeRedLightPulsing.part
				
	End
	
End

#########################################################################
	
Condition
	On 	Time
	Time 	15



	Event
		Type	Local 
		At	lightgeom
				
		BHVR	:RunwaylightOffset4.bhvr	
		altpiv	2
		
		Part	:LargeRedLightPulsing.part
		Part	:LargeRedLightPulsing.part
				
	End
	
End

#########################################################################
	
Condition
	On 	Time
	Time 	20



	Event
		Type	Local 
		At	lightgeom
				
		BHVR	:RunwaylightOffset5.bhvr	
		altpiv	2
		
		Part	:LargeRedLightPulsing.part
		Part	:LargeRedLightPulsing.part
				
	End
	
End
