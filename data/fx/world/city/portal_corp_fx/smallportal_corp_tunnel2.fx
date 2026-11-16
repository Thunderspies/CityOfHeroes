#########################################################
##	template

FxInfo

Flags    InheritAlpha
   
#########################################################
              

	
Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound glowroom_loop 60 60 .82
	End
End

Condition
	On	Cycle
	Time 	60	

	Event
		Type	local 
		At	Origin
		#Part	:SmalOrbGlowPulse.part
		Lifespan 30
	End
End

Condition
	Event
		Type	start 
		At	Origin	
		
		#Part	:SmalOrbGlowConstant.part
	
		#Part	:ElectricityArchSmal2.part
		#Part	:ElectricityArchSmal.part
		
		#Part	:ElectricitySmal01.part
		#Part	:ElectricitySmal02.part
		Part	:SmalInwardGlow01.part
		#Sound	reactor2_loop 100 70 .4
		
	End
End

Condition
	Event
		Type	start 
		At	Origin	

		Part	:SmalOrbOuterRingSpark01.part
		Part	:SmalOrbOuterRingSpark02.part
		Part	World/Lairs/Cot/PortalRings2.part
		Bhvr	:SmallPortal_Corp_AltPivot.bhvr
	End
End

Condition
	On	Cycle
	Time 	60	

	Event
		Type	local 
		At	Origin
		Part	:SmalInwardGlowDark.part
		Bhvr	:SmallPortal_Camera_Shake.bhvr
		Lifespan 30
	End
End

End