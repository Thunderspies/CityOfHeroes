#########################################################
##	Portal
########################################################
FxInfo


Condition

	Event
		Type	local 
		At	root
		Sound Bportal_loop 70 70 .5
		
	End
End


Condition

	Event
		ENAME	OFFSET
		Type	local 
		At	root
		bhvr	:OffsetPortal.bhvr
		
		
	End

	Event
		Type	start 
		At	OFFSET
		bhvr	:Rune_spin.bhvr
		Part	:Pantheon_Runic_Emblem1.part
		
	End
	
	Event
		Type	start 
		At	OFFSET
		bhvr	:Rune_spin2.bhvr
		Part	:Pantheon_Runic_Emblem2.part
		
	End
	
	Event
		Type	start 
		At	OFFSET
		bhvr	:Rune_spin3.bhvr
		Part	:Pantheon_Runic_Emblem3.part
		
	End
	
	Event
		Type	start 
		At	OFFSET
		bhvr	:Rune_spin4.bhvr
		Part	:Pantheon_Runic_Emblem4.part
		
	End

	Event
		Type	start 
		At	OFFSET
		Part	:PortalCore.part
		Part	:PoetalGasses.part
		Part	:PoetalGassesDefuse.part
		
		#Part	:Pantheon_Runic_Glow2.part
	End

End

End
