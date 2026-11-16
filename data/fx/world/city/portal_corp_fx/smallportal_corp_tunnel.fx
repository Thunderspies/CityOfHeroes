#########################################################
##	template

FxInfo

Flags    InheritAlpha
   
#########################################################
              

Condition	
	Event	Type	start
		At	origin
		Sound reactor1_loop 100 100 .6
	End
End


Condition	
	Event	Type	start
		At	origin
		Sound lowpulse2_loop 100 100 .4
	End
End

Condition	
	Event	Type	start
		At	origin
		Sound teslaroom4_loop 100 100 .4
	End
End


Condition	
	Event	Type	start
		At	origin
		Sound spacemachine1m_loop 100 100 .6
	End
End

Condition	
	Event	Type	start
		At	origin
		Sound train_loop 100 100 .44
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
		Sound reactor2_loop 100 100 .4
		
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