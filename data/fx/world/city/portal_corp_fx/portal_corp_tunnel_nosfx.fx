#########################################################
##	template

FxInfo

Flags    InheritAlpha
   
#########################################################
              

Condition	
	Event	Type	start
		At	origin
		#Sound reactor1_loop 340 340 .99
	End
End


Condition	
	Event	Type	start
		At	origin
		#Sound rumble1_loop 120 120 .33
	End
End

Condition	
	Event	Type	start
		At	origin
		#Sound lowpulse2_loop 210 210 .7
	End
End

Condition	
	Event	Type	start
		At	origin
		#Sound teslaroom4_loop 190 190 .99
	End
End


Condition	
	Event	Type	start
		At	origin
		#Sound spacemachine1m_loop 160 160 .99
	End
End

Condition	
	Event	Type	start
		At	origin
		#Sound train_loop 320 320 .7
	End
End

Condition
	On	Cycle
	Time 	30	

	Event
		Type	local 
		At	Origin

		Geom	Portal_Corp_Inner
		#bhvr	:Portal_Tunnel_01.bhvr
		Lifespan 30
	End
End

Condition
	On	Cycle
	Time 	60	

	Event
		Type	local 
		At	Origin
		Part	:OrbGlowPulse.part
		Lifespan 30
	End
End

Condition
	Event
		Type	start 
		At	Origin	
		
		Part	:OrbGlowConstant.part
	
		Part	:ElectricityArchLarge2.part
		Part	:ElectricityArchLarge.part

		Part	:ElectricityBig01.part
		Part	:InwardGlow01.part
		#Sound reactor2_loop 330 330 .7
		
	End
End

Condition
	Event
		Type	start 
		At	Origin	

		Part	:OrbOuterRingSpark01.part
		Part	:OrbOuterRingSpark02.part
		Part	:OrbOuterRingSpark03.part
		Bhvr	:Portal_Corp_AltPivot.bhvr
	End
End

Condition
	On	Cycle
	Time 	60	

	Event
		Type	local 
		At	Origin
		Part	:InwardGlowDark.part
		Bhvr	:Portal_Camera_Shake.bhvr
		Lifespan 30
	End
End

End