
FxInfo


##################################

Condition
	On 	Time
	Time 	0



	Event
		EName	base1
		Type	Local
		At	hips
#		Geom	XrayBeam
		bhvr	:tail_root.bhvr

		End

		


	Event
		EName	Seg1F
		Type	Local
		At	base1
#		Geom	testing_target
#		bhvr	:squish.bhvr

	End


	Event
		EName	Seg1
		Type	Local
		At	Seg1F
#		Geom	testing_target
#		bhvr	:spin.bhvr

	End

	

	Event
		EName	Targ1
		Type	Local
		At	Seg1
#		Geom	testing_target
		bhvr	:targetX.bhvr

	End

	Event
		EName	Targ1b
		Type	Local
		At	Targ1
#		Geom	testing_target
		bhvr	:targetX.bhvr

	End
	

	Event
		EName	seek1
		Type	start
		At	root
#		Geom	testing_target
		bhvr	:seek.bhvr
		magnet	Targ1b

	End

	


	Event
		EName	Aimer1
		Type	positonly
		At	base1
		lookat seek1
	End

	Event
		EName	Off1
		Type	local
		At	Aimer1
#		Geom	testing_target
		bhvr	:Jointoffset.bhvr
	End

	Event
		EName	Geom1
		Type	local
		At	Aimer1
		Geom	XrayBeam2
		bhvr	:tail_resize.bhvr
		End

End		
##################################

Condition
	On 	Time
	Time 	0



	Event
		EName	base2
		Type	Local
		At	off1
#		Geom	XrayBeam2
		bhvr	:tail_root.bhvr

		End

		


	Event
		EName	Seg2F
		Type	Local
		At	base2
#		Geom	testing_target
#		bhvr	:squish.bhvr

	End


	Event
		EName	Seg2
		Type	Local
		At	Seg2F
##		Geom	testing_target
#		bhvr	:spin2.bhvr

	End

	

	Event
		EName	Targ2
		Type	Local
		At	Seg2
#		Geom	testing_target
		bhvr	:targetX.bhvr

	End

	Event
		EName	Targ2b
		Type	Local
		At	Targ2
#		Geom	testing_target
		bhvr	:targetX.bhvr

	End
	

	Event
		EName	seek2
		Type	start
		At	root
#		Geom	testing_target
		bhvr	:seek.bhvr
		magnet	Targ2b

	End

	


	Event
		EName	Aimer2
		Type	positonly
		At	base2
		lookat seek2
	End

	Event
		EName	Off2
		Type	local
		At	Aimer2
#		Geom	testing_target
		bhvr	:Jointoffset.bhvr
	End

	Event
		EName	Geom2
		Type	local
		At	Aimer2
		Geom	XrayBeam2
		bhvr	:tail_resize.bhvr
		End

End		

		
##################################

Condition
	On 	Time
	Time 	0



	Event
		EName	base3
		Type	Local
		At	off2
#		Geom	XrayBeam2
		bhvr	:tail_root.bhvr

		End

		


	Event
		EName	Seg3F
		Type	Local
		At	base3
#		Geom	testing_target
#		bhvr	:squish.bhvr

	End


	Event
		EName	Seg3
		Type	Local
		At	Seg3F
#		Geom	testing_target
#		bhvr	:spin3.bhvr

	End

	

	Event
		EName	Targ3
		Type	Local
		At	Seg3
#		Geom	testing_target
		bhvr	:targetX.bhvr

	End

	Event
		EName	Targ3b
		Type	Local
		At	Targ3
#		Geom	testing_target
		bhvr	:targetX.bhvr

	End
	

	Event
		EName	seek3
		Type	start
		At	root
#		Geom	testing_target
		bhvr	:seek.bhvr
		magnet	Targ3b

	End

	


	Event
		EName	Aimer3
		Type	positonly
		At	base3
		lookat seek3
	End

	Event
		EName	Off3
		Type	local
		At	Aimer3
#		Geom	testing_target
		bhvr	:Jointoffset.bhvr
	End

	Event
		EName	Geom3
		Type	local
		At	Aimer3
		Geom	XrayBeam2
		bhvr	:tail_resize.bhvr
		End

End		

