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
		
		Part	:SnowFall_Blizzard_Ground.part
		Part	:SnowFall_Blizzard_Glow_Ground.part
		Part	:SnowFall_Blizzard2_Ground.part
		Part	:SnowFall_Blizzard3_Ground.part
#		Part	:SnowFall_Blizzard_Ground.part
#		Part	:SnowFall_Blizzard_Glow_Ground.part
#		Part	:SnowFall_Blizzard2_Ground.part
#		Part	:SnowFall_Blizzard3_Ground.part
#		Part	:SnowFall_Blizzard_Ground.part
#		Part	:SnowFall_Blizzard_Glow_Ground.part
#		Part	:SnowFall_Blizzard2_Ground.part
#		Part	:SnowFall_Blizzard3_Ground.part
			
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
		
		Part	:SnowFall_Blizzard.part
		Part	:SnowFall_Blizzard_Glow.part
		Part	:SnowFall_Blizzard2.part
		Part	:SnowFall_Blizzard3.part
#		Part	:SnowFall_Blizzard.part
#		Part	:SnowFall_Blizzard_Glow.part
#		Part	:SnowFall_Blizzard2.part
#		Part	:SnowFall_Blizzard3.part
#		Part	:SnowFall_Blizzard.part
#		Part	:SnowFall_Blizzard_Glow.part
#		Part	:SnowFall_Blizzard2.part
#		Part	:SnowFall_Blizzard3.part
#		Part	:SnowFall_Blizzard.part
#		Part	:SnowFall_Blizzard_Glow.part
#		Part	:SnowFall_Blizzard2.part
#		Part	:SnowFall_Blizzard3.part
		
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
		
		Part	:SnowSpeed_Blizzard.part
		Part	:SnowSpeed_Blizzard_Glow.part
		Part	:SnowSpeed_Blizzard2.part
		Part	:SnowSpeed_Blizzard3.part
#		Part	:SnowSpeed_Blizzard.part
#		Part	:SnowSpeed_Blizzard_Glow.part
#		Part	:SnowSpeed_Blizzard2.part
#		Part	:SnowSpeed_Blizzard3.part
#		Part	:SnowSpeed_Blizzard.part
#		Part	:SnowSpeed_Blizzard_Glow.part
#		Part	:SnowSpeed_Blizzard2.part
#		Part	:SnowSpeed_Blizzard3.part
#		Part	:SnowSpeed_Blizzard.part
#		Part	:SnowSpeed_Blizzard_Glow.part
#		Part	:SnowSpeed_Blizzard2.part
#		Part	:SnowSpeed_Blizzard3.part
		
	End
	
End

End		