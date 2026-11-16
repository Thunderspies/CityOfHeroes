#########################################################
##	RadiationAura
FxInfo

Input  
	InpName	root
End


##################################

Condition
#	On	cycle
#	Time	5
	
	Event
		Type	Local
		At	root
		
		Part	:SnowFall_Ground.part
		Part	:SnowFall_Glow_Ground.part
		Part	:SnowFall2_Ground.part
		Part	:SnowFall3_Ground.part
			
	End

End

Condition
	On	Time
	Time	0
	
	Event
		EName	Hookup
		Type	Local
		At	root
		
	End

	Event
		Type	local
		At	Hookup
		
		Part	:SnowFall.part
		Part	:SnowFall_Glow.part
		Part	:SnowFall2.part
		Part	:SnowFall3.part
		Part	:SnowFall.part
		Part	:SnowFall_Glow.part
		Part	:SnowFall2.part
		Part	:SnowFall3.part
		Part	:SnowFall.part
		Part	:SnowFall_Glow.part
		Part	:SnowFall2.part
		Part	:SnowFall3.part
		Part	:SnowFall.part
		Part	:SnowFall_Glow.part
		Part	:SnowFall2.part
		Part	:SnowFall3.part
		
	End
		Event
		EName	Hookup2
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 5 30
		End
		#Sound	blizzard2 100 30 .9
		
	End

	Event
		Type	local
		At	Hookup2
		
		Part	:SnowSpeed.part
		Part	:SnowSpeed_Glow.part
		Part	:SnowSpeed2.part
		Part	:SnowSpeed3.part
		Part	:SnowSpeed.part
		Part	:SnowSpeed.part
		Part	:SnowSpeed2.part
		Part	:SnowSpeed3.part
		Part	:SnowSpeed.part
		Part	:SnowSpeed_Glow.part
		Part	:SnowSpeed2.part
		Part	:SnowSpeed3.part
		Part	:SnowSpeed.part
		Part	:SnowSpeed_Glow.part
		Part	:SnowSpeed2.part
		Part	:SnowSpeed3.part
		
	End
	
End

End		