#########################################################
##	jack
##
FxInfo

Flags  InheritAlpha  # InheritAnimScale

Input
	InpName	hips
End


Condition
	On Time
	Time 0
	Event
		Type	Local
		At	Origin
		Sound Sharkcircle2_loop 70 70 .5
	End
End


Condition
	On 	Time
	Time 	0
	Event
		ename	start1
		Type	local
		At	root
		bhvr	V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_CircleShrink.bhvr
	End

	Event
		ename	start2
		Type	local
		At	root
		bhvr	V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_CircleShrink.bhvr
	End

	Event
		ename	raiser1
		Type	local
		At	start1
		Bhvroverride
			PositionOffset 0 -1.7 0
			startjitter 0 1 0
			FadeOutLength		160
		end
		magnet	root
	End

	Event
		ename	Holder1
		Type	positonly
		At	raiser1
		bhvr	V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_SpiritsharkFadeout.bhvr
	End

	Event
		ename	raiser2
		Type	local
		At	start1
		Bhvroverride
			PositionOffset 0 -1.7 0
			startjitter 0 1 0
			FadeOutLength		160
		end
		magnet	root
	End

	Event
		ename	Holder2
		Type	positonly
		At	raiser2
		bhvr	V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_SpiritsharkFadeout.bhvr
	End


	Event
		ename	shark1
		Type	local
		At	Holder1
		Anim	Shark_Spirit_Circle_Tintable
		bhvr	V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_SpiritsharkFadeout.bhvr
		Bhvroverride
			scale .3 .3 .3
#			PositionOffset	0 -1.7 0
			spin 0 -.04 0
			FadeOutLength	120
			FadeInLength	120
			TintGeom	1
		end
	End

	Event
		ename	shark2
		Type	local
		At	Holder2
		Anim	Shark_Spirit_Circle_Tintable ##ent type

		bhvr	V_COV\EnemyPowers\ArchVillains\Mako\V_Mako_SpiritsharkFadeout.bhvr
		Bhvroverride
			scale .3 .3 .3
			pyrrotate 0 180 0
#			PositionOffset	0 -1.7 0
			spin 0 -.04 0
			TintGeom	1
		end
	End

	Event
		Type	Local
		At	shark1_Tintable
		AnimPiv chest
		part	:V_Mako_Spiritshark_wake1.part
		pmagnet	root
	End

	Event
		Type	Local
		At	shark1
		AnimPiv footL
		part	:V_Mako_Spiritshark_wake2.part
		pmagnet	root
	End
	Event
		Type	Local
		At	shark2
		AnimPiv chest
		part	:V_Mako_Spiritshark_wake1.part
		pmagnet	root
	End

	Event
		Type	Local
		At	shark2
		AnimPiv footL
		part	:V_Mako_Spiritshark_wake2.part
		pmagnet	root
	End

end


