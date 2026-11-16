#########################################################
##	forceBubble
########################################################
FxInfo


LifeSpan	65    

########################################################


######################################################################################



Condition
	On 	Time
	Time 	0
	
	Event
		Ename	Hookup
		Type	local
		At	Root
		BHVR	:OverheadOffset.bhvr

	End

	Event
		Type	local
		At	Hookup
		Part	:LightningStrike2.part	
		Pmagnet	 root

	End	

######################################################################################

	Event
		Ename	Hookup2
		Type	local
		At	Root
		BHVR	:OverheadOffset2.bhvr

	End

	Event
		Type	local
		At	Hookup2
		Part	:LightningStrike2.part	
		Pmagnet	 root

	End	
End


Condition
	On 	Time
	Time 	3

	Event
		Type	local
		At	Hookup
		Part	:LightningStrike3.part	
		Pmagnet	 root

	End	
	
######################################################################################
End


Condition
	On 	Time
	Time 	5
	Event
		Type	local
		At	Hookup2
		Part	:LightningStrike2.part	
		Pmagnet	 root

	End	

End


Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Hookup
		Part	:LightningStrike.part
		Pmagnet	 root	

	End	
	
	Event
		Type	local
		At	Hookup
		Part	:LightningStrikeInvert.part
		Pmagnet	 root

	End	

######################################################################################


	Event
		Type	local
		At	Hookup2
		Part	:LightningStrike.part
		Pmagnet	 root	

	End	
	
	Event
		Type	local
		At	Hookup2
		Part	:LightningStrikeInvert.part
		Pmagnet	 root

	End	


End

######################################################################################

Condition
	On 	Time
	Time 	2
	Event
		Type	local
		At	Chest
		Part	:LightningStrikeGlow.part
		Part	:LightningStrikeHaze.part
		Lifespan		10
	End	

End


Condition
	On 	Time
	Time 	3

	Event
		Type	local
		At	Chest	
		Part	:LightningStreeks.part
		Part	:LightningStreeksDark.part

		Lifespan		12
	End

End



End