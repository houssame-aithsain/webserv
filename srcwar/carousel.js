const carouselitems = document.querySelectorAll(".carousel-item");
const imgitem = document.querySelectorAll(".list-inline-item");
console.log(imgitem)
carouselitems.forEach((e) => {
  if (e.classList.contains("active")) e.style.display = "block";
  else e.style.display = "none";
});

function sleep(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

imgitem.forEach((e) => {
  e.addEventListener("click", (item) => {
    carouselitems.forEach((i) => {
      console.log(carouselitems)
      if (i.classList.contains("active")) {
        replace(i, e)
      }
      else
        i.firstElementChild.style.opacity = 0;
    });
  });
});

async function replace(i, e) {
  if (i.classList.contains("active")) 
  {
    i.firstElementChild.style.opacity = 0;
    i.firstElementChild.src = e.firstElementChild.firstElementChild.src;
    i.firstElementChild.style.opacity = 1;
  }
}

const up = document.querySelector(".fa-angle-up");
const down = document.querySelector(".fa-angle-down");
const items = document.querySelector(".carousel-indicators");
let i = 1;

up.addEventListener("click", () => {
    if (i % 2 != 0) {
      items.scrollTop = items.scrollHeight;
      i++;
      } else {
      items.scrollTop = 0;
      i = 1;
    }
});

down.addEventListener("click", () => {

    if (i % 2 != 0) {
      items.scrollTop = items.scrollHeight;
      i++;
    } else {
      items.scrollTop = 0;
      i = 1;
    }
});


// const desc = document.querySelector('.head_wrapper_desc');
// let counter = 0;

// desc.addEventListener('click', async()=>{
//   let wrap_desc = document.querySelector('.wrapper_descriptionnn_');
//   let content = document.querySelector('._desc_content_wrapper');
//   if (counter == 0)
//   {
//     wrap_desc.style.height = "60vh";
//     desc.children[1].children[0].classList.erreplace('fa-angle-down','fa-angle-up')
//     await sleep(200);
//     content.style.display = "block";
//     counter++;
//   }
//   else
//   {
//     wrap_desc.style.height = "65px";
//     desc.children[1  ].children[0].classList.replace('fa-angle-up','fa-angle-down')
//     await sleep(200);
//     content.style.display = "none";
//     counter = 0;
//   }
// })



  // const insider = document.querySelectorAll('.bar_insider');
  // const content_in = document.querySelectorAll('.nbr_reviews');

  // insider.forEach((e)=>{
  //   e.style.width = Math.floor(Math.random() * (200 - 50 + 1)) + 50 +"px";
  // })

  // content_in.forEach((e)=>{
  //   e.innerHTML = Math.floor(Math.random() * (200 - 150 + 1));
  // })


  // const au = document.querySelector('.fa-angle-up')
  // const ad = document.querySelector('.fa-angle-down')

  // const angleHandler = () => {
  //   if (window.innerWidth <= 800 )
  //   {
  //     au.classList.replace('fa-angle-up', 'fa-angle-left');
  //     ad.classList.replace('fa-angle-down', 'fa-angle-right');
  //   }
  //   else
  //   { 
  //     au.classList.replace('fa-angle-left', 'fa-angle-up');
  //     ad.classList.replace('fa-angle-right', 'fa-angle-down');
  //   }
  // }

  // window.addEventListener('resize', angleHandler);

  // window.addEventListener('load', angleHandler);x²