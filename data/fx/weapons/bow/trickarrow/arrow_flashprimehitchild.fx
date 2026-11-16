#########################################################
##	template

FxInfo


Flags InheritAnimScale InheritAlpha

LifeSpan 300

#### arrow in hand  ########################

Condition
	On	cycle
	Time	2

	Event	
		ENAME	Flash
		Type	Local
		At	origin
		
		Bhvr	:FlashBangGlowSplat.bhvr
		Splat	Glow.tga

#		part	:FlashBangFlashFlat.part
		LifeSpan 12	
	End

End

Condition
	On	cycle
	Time	2

	Event	
		ENAME	Flash
		Type	Local
		At	origin
		
		Bhvr	:FlashBangGlowSplat.bhvr
		Splat	Glow.tga

#		part	:FlashBangFlashFlat.part
		LifeSpan 12	
	End

End

Condition
	On	cycle
	Time	2

	Event	
		ENAME	Flash
		Type	Local
		At	origin
		
		Bhvr	:FlashBangGlowSplat.bhvr
		Splat	Glow.tga

#		part	:FlashBangFlashFlat.part
		LifeSpan 12	
	End

End

End		