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
		At	root
		Geom	RunwayLightOrigin
								
	End

End

	
Condition
	On 	cycle
	Time 	10

	Event
		Type	Local 
		At	lightgeom
		
		altpiv	1		
		BHVR	:RunwaylightOffset1.bhvr
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5		
	End

	Event
		Type	Local 
		At	lightgeom
				
		altpiv	2
		BHVR	:RunwaylightOffset1.bhvr
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5			
	End

End

#########################################################################
	

Condition
	On 	cycle
	Time 	15

	Event
		Type	Local 
		At	lightgeom
		
		BHVR	:RunwaylightOffset.bhvr		
		altpiv	1
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End

	Event
		Type	Local 
		At	lightgeom
				
		BHVR	:RunwaylightOffset.bhvr	
		altpiv	2
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End
	
End

#########################################################################

Condition
	On 	cycle
	Time 	20

	Event
		Type	Local 
		At	lightgeom
		
		BHVR	:RunwaylightOffset3.bhvr		
		altpiv	1
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End

	Event
		Type	Local 
		At	lightgeom
				
		BHVR	:RunwaylightOffset3.bhvr	
		altpiv	2
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End
	
End

#########################################################################
	

Condition
	On 	cycle
	Time 	25

	Event
		Type	Local 
		At	lightgeom
		
		BHVR	:RunwaylightOffset4.bhvr		
		altpiv	1
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End

	Event
		Type	Local 
		At	lightgeom
				
		BHVR	:RunwaylightOffset4.bhvr	
		altpiv	2
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End
	
End

#########################################################################
	

Condition
	On 	cycle
	Time 	30

	Event
		Type	Local 
		At	lightgeom
		
		BHVR	:RunwaylightOffset5.bhvr		
		altpiv	1
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End

	Event
		Type	Local 
		At	lightgeom
				
		BHVR	:RunwaylightOffset5.bhvr	
		altpiv	2
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End
	
End

#########################################################################

Condition
	On 	cycle
	Time 	35

	Event
		Type	Local 
		At	lightgeom
		
		BHVR	:RunwaylightOffset6.bhvr		
		altpiv	1
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End

	Event
		Type	Local 
		At	lightgeom
				
		BHVR	:RunwaylightOffset6.bhvr	
		altpiv	2
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End
	
End

#########################################################################
	

Condition
	On 	cycle
	Time 	40

	Event
		Type	Local 
		At	lightgeom
		
		BHVR	:RunwaylightOffset7.bhvr		
		altpiv	1
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End

	Event
		Type	Local 
		At	lightgeom
				
		BHVR	:RunwaylightOffset7.bhvr	
		altpiv	2
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End
	
End

#########################################################################

Condition
	On 	cycle
	Time 	45

	Event
		Type	Local 
		At	lightgeom
		
		BHVR	:RunwaylightOffset8.bhvr		
		altpiv	1
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End

	Event
		Type	Local 
		At	lightgeom
				
		BHVR	:RunwaylightOffset8.bhvr	
		altpiv	2
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End
	
End

#########################################################################

Condition
	On 	cycle
	Time 	50

	Event
		Type	Local 
		At	lightgeom
		
		BHVR	:RunwaylightOffset9.bhvr		
		altpiv	1
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End

	Event
		Type	Local 
		At	lightgeom
				
		BHVR	:RunwaylightOffset9.bhvr	
		altpiv	2
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End
	
End

#########################################################################

Condition
	On 	cycle
	Time 	55

	Event
		Type	Local 
		At	lightgeom
		
		BHVR	:RunwaylightOffset10.bhvr		
		altpiv	1
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End

	Event
		Type	Local 
		At	lightgeom
				
		BHVR	:RunwaylightOffset10.bhvr	
		altpiv	2
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End
	
End

#########################################################################

Condition
	On 	cycle
	Time 	60

	Event
		Type	Local 
		At	lightgeom
		
		BHVR	:RunwaylightOffset11.bhvr		
		altpiv	1
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End

	Event
		Type	Local 
		At	lightgeom
				
		BHVR	:RunwaylightOffset11.bhvr	
		altpiv	2
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End
	
End

#########################################################################

Condition
	On 	cycle
	Time 	65

	Event
		Type	Local 
		At	lightgeom
		
		BHVR	:RunwaylightOffset12.bhvr		
		altpiv	1
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End

	Event
		Type	Local 
		At	lightgeom
				
		BHVR	:RunwaylightOffset12.bhvr	
		altpiv	2
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End
	
End

#########################################################################

Condition
	On 	cycle
	Time 	70

	Event
		Type	Local 
		At	lightgeom
		
		BHVR	:RunwaylightOffset13.bhvr		
		altpiv	1
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End

	Event
		Type	Local 
		At	lightgeom
				
		BHVR	:RunwaylightOffset13.bhvr	
		altpiv	2
		
		Part	:LargeBlueLightPulsingFast.part
		Part	:LargeWhiteLightPulsingFast.part
		LifeSpan	5	
				
	End
	
End

#######################################################################
###########################################################

End			