#############################################################
## StickyArrowCont.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	arrow
		Type	Local
		At	LlegL
		Part	:Sticky2.part
		Part	:StickyDark2.part
	End

	Event
		EName 	arrow
		Type	Local
		At	FootL
		Part	:Sticky.part
		Part	:StickyDark.part
#		Sound	soot2_loop 70 60 0.36
	End

	Event
		EName 	arrow
		Type	Local
		At	LlegR
		Part	:Sticky2.part
		Part	:StickyDark2.part
	End

	Event
		EName 	arrow
		Type	Local
		At	FootR
		Part	:Sticky.part
		Part	:StickyDark.part
		Sound goo_loop 70 70 0.1
	End
End

#############################################################

End